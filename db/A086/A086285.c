/*
 * A086285  Numbers k such that 1 + 2k + 3k^2 is prime.
 * https://oeis.org/A086285
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mul64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A086285: Numbers k such that 1 + 2k + 3k² is prime
    This routine searches k in increasing order, keeps a cache of
    the found terms and returns the n‑th such k (offset 1).  */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/mul64.h"

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/*  64‑bit modular exponentiation                                       */
static inline uint64_t powmod(uint64_t a, uint64_t e, uint64_t mod)
{
    uint64_t r = 1;
    while (e) {
        if (e & 1) r = mul64(r, a, mod);
        a   = mul64(a, a, mod);
        e >>= 1;
    }
    return r;
}

/* ------------------------------------------------------------------ */
/*  Deterministic Miller–Rabin test for 64‑bit integers                */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;

    /* Trial division by small primes */
    const uint64_t small_primes[] =
        {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        uint64_t p = small_primes[i];
        if (n % p == 0) return n == p;
    }

    /* Decompose n-1 = d * 2^s */
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    /* Witnesses adequate for 64‑bit numbers */
    const uint64_t bases[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i];
        if (a >= n) break;
        uint64_t x = powmod(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = mul64(x, x, n);
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/*  Main sequence routine                                               */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                /* undefined for offset 0 */

    static seq_t *cache = NULL;
    static size_t cache_len = 0, cache_cap = 0;
    static uint64_t candidate = 2;       /* first k that may work */

    /* grow cache to accommodate requested index */
    if (cache_len < (size_t)n) {
        if (cache_cap == 0) {
            cache_cap = 8;
            cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
            if (!cache) return 0; /* allocation failure – fallback value */
        }
        while (cache_len < (size_t)n) {
            /* compute P(k) = 1 + 2k + 3k^2 */
            uint64_t k = candidate;
            unsigned __int128 val128 = (unsigned __int128)3 * k * k
                                    + (unsigned __int128)2 * k
                                    + (unsigned __int128)1;
            uint64_t val = (uint64_t)val128;   /* fits for all k we will reach */

            if (is_prime_uint64(val)) {
                if (cache_len == cache_cap) {
                    cache_cap <<= 1;
                    seq_t *tmp = (seq_t *)realloc(cache,
                                                  cache_cap * sizeof(seq_t));
                    if (!tmp) { free(cache); return 0; }
                    cache = tmp;
                }
                cache[cache_len++] = (seq_t)k;
            }
            ++candidate;
        }
    }
    return cache[n-1];
}
