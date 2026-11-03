/*
 * A210504  Numbers n for which 2*n+5, 4*n+5, 6*n+5, and 8*n+5 are primes.
 * https://oeis.org/A210504
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth integer m such that 2*m+5, 4*m+5, 6*m+5, and
 *            8*m+5 are all prime. The sequence is indexed from 0
 * Range:     u8 n <= 4, u16 n <= 65, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 64 terms through n = 945 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* Small primes used for a quick pre‑filter.
   They are larger than 3 and cover a wheel that eliminates
   the majority of composites early. */
static const uint16_t SMALL_PRIMES[] = {
    5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47
};
#define N_SMALL (sizeof(SMALL_PRIMES)/sizeof(SMALL_PRIMES[0]))

/* Cached terms of the sequence. */
static seq_t *cache = NULL;
static size_t cache_len = 0;      /* number of valid entries */
static size_t cache_cap = 0;      /* allocated capacity */

/* The next multiple of 3 to test.  All terms are multiples of 3. */
static seq_t current_candidate = 0;

/* Stored residues of current_candidate modulo the small primes.
   len == N_SMALL, residue[i] == current_candidate % SMALL_PRIMES[i]. */
static uint16_t residues[N_SMALL] = {0};

/* Ensure that cache contains at least (index+1) elements. */
static void ensure_cache(idx_t index)
{
    /* Grow the cache if necessary. */
    while (cache_cap < (size_t)(index + 1)) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp) abort();               /* fatal: out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Generate terms until we have enough. */
    while (cache_len <= (size_t)index) {
        seq_t n = current_candidate;

        /* Compute the four linear expressions. */
        seq_t v2 = 2 * n + 5;   /* 2*n+5 */
        seq_t v4 = 4 * n + 5;   /* 4*n+5 */
        seq_t v6 = 6 * n + 5;   /* 6*n+5 */
        seq_t v8 = 8 * n + 5;   /* 8*n+5 */

        /* Quick composite filter using the small primes. */
        bool bad = false;
        for (size_t i = 0; i < N_SMALL && !bad; ++i) {
            uint16_t p = SMALL_PRIMES[i];

            /* If a value equals the small prime, it is allowed. */
            if ((v2 % p == 0) && (v2 != (seq_t)p)) bad = true;
            else if ((v4 % p == 0) && (v4 != (seq_t)p)) bad = true;
            else if ((v6 % p == 0) && (v6 != (seq_t)p)) bad = true;
            else if ((v8 % p == 0) && (v8 != (seq_t)p)) bad = true;
        }

        /* If it passes the filter, test the four primes. */
        if (!bad) {
            if (is_prime_big(v2) && is_prime_big(v4) &&
                is_prime_big(v6) && is_prime_big(v8)) {
                /* Store the qualifying value. */
                cache[cache_len++] = n;
            }
        }

        /* Advance to the next candidate (next multiple of 3). */
        current_candidate += 3;

        /* Update residues for the next iteration. */
        for (size_t i = 0; i < N_SMALL; ++i) {
            residues[i] += 3;
            if (residues[i] >= SMALL_PRIMES[i]) residues[i] -= SMALL_PRIMES[i];
        }
    }
}

/* The exported sequence function. */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
