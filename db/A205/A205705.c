/*
 * A205705  Numbers k for which 8 divides prime(k)-prime(j) for some j<k; each
 *          k occurs once for each such j.
 * https://oeis.org/A205705
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   alloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A205705: Numbers k such that 8 divides prime(k)-prime(j) for some j<k.
   Each occurrence of k in the sequence corresponds to one j satisfying the
   condition.  The implementation generates primes incrementally, tracks
   how many previous primes share each residue modulo 8, and outputs the
   current prime index k once for each earlier prime with the same residue.
   The sequence is built lazily and cached, so any call a(n) for n≥1
   returns the correct term regardless of call order. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Use the general allocation functions if available,
   otherwise fall back to standard malloc/realloc */
#if defined(HAVE_SEQ_ALLOC) && defined(HAVE_SEQ_REALLOC)
#include "../utils/alloc.h"  /* assumes seq_alloc and seq_realloc are declared */
#else
static void *seq_alloc(size_t cap)  { return malloc(cap); }
static void *seq_realloc(void *old, size_t, size_t new_cap) { return realloc(old, new_cap); }
#endif

/* ---------- Helper: simple prime test using the list of known primes ---------- */
static bool is_prime_uint32(uint32_t x, const uint32_t *primes, size_t primes_len) {
    if (x < 2) return false;
    for (size_t i = 0; i < primes_len; ++i) {
        uint32_t p = primes[i];
        if ((uint64_t)p * (uint64_t)p > (uint64_t)x) break;
        if (x % p == 0) return false;
    }
    return true;
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    static seq_t *cache          = NULL;    /* cached sequence terms */
    static size_t cache_len      = 0;
    static size_t cache_cap      = 0;

    static uint32_t *primes      = NULL;    /* list of primes */
    static size_t primes_len     = 0;
    static size_t primes_cap     = 0;

    static uint32_t next_candidate = 2;      /* number to test next */
    static uint8_t residue_counts[8] = {0};

    /* Grow the cache when needed */
    if (cache == NULL) {
        cache_cap = 256;
        cache = (seq_t *)seq_alloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) abort();
    }
    if (primes == NULL) {
        primes_cap = 256;
        primes = (uint32_t *)seq_alloc(primes_cap * sizeof(uint32_t));
        if (primes == NULL) abort();
    }

    /* Generate terms until we have at least n terms */
    while (cache_len < (size_t)n) {
        /* Find the next prime */
        uint32_t prime;
        for (;;) {
            if (is_prime_uint32(next_candidate, primes, primes_len)) {
                prime = next_candidate;
                break;
            }
            ++next_candidate;
        }
        /* Append to prime list, growing it if necessary */
        if (primes_len == primes_cap) {
            size_t new_cap = primes_cap << 1U;
            uint32_t *tmp = (uint32_t *)seq_realloc(primes, primes_cap, new_cap);
            if (tmp == NULL) abort();
            primes = tmp;
            primes_cap = new_cap;
        }
        primes[primes_len++] = prime;
        next_candidate = prime + 1;   /* next candidate after this prime */

        /* Current prime index (1‑based) */
        uint32_t prime_index = (uint32_t)primes_len;

        /* Residue modulo 8 of current prime */
        uint8_t residue = prime & 7U;
        uint8_t count = residue_counts[residue];

        /* Emit the current prime index once for each earlier prime with same residue */
        for (uint8_t i = 0; i < count; ++i) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap << 1U;
                seq_t *tmp = (seq_t *)seq_realloc(cache, cache_cap, new_cap);
                if (tmp == NULL) abort();
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)prime_index;
        }

        /* Update residue count for future primes */
        residue_counts[residue] = count + 1;
    }

    return cache[(size_t)n - 1];
}
