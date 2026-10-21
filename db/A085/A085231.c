/*
 * A085231  Numbers k in whose canonical factorization the power of the
 *          smallest prime factor is greater than the power of the greatest
 *          prime factor.
 * https://oeis.org/A085231
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A085231 – numbers k whose canonical factorization satisfies
    p^e > q^f where p is the smallest prime factor with exponent e
    and q the largest prime factor with exponent f.
    The function builds the sequence on demand, caching results for
    random‑order access. */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"

/* cache of generated terms */
static seq_t *cache      = NULL;    /* array of cached terms */
static size_t cache_cap  = 0;       /* allocated capacity */
static idx_t  generated  = 0;       /* number of terms cached */

/* ensures the cache has at least “need” slots */
static void ensure_cache_cap(idx_t need)
{
    if ((idx_t)cache_cap >= need)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < (size_t)need)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
    cache = tmp;
    cache_cap = new_cap;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)                          /* undefined for n<1 */
        return (seq_t)0;

    ensure_cache_cap(n);                 /* grow cache if needed */

    /* next integer to test – persistent across calls */
    static uint64_t current_val = 12;

    while (generated < n) {
        uint64_t term = current_val;

        /* factorization: primes in `pr`, exponents in `ex` */
        uint64_t pr[64];
        uint64_t ex[64];
        size_t   cnt = 0;
        factor_u64(term, pr, ex, &cnt);

        /* identify smallest and largest prime factors and their exponents */
        if (cnt > 0) {
            uint64_t   smallest = UINT64_MAX, largest = 0;
            uint64_t   exp_small = 0, exp_large = 0;

            for (size_t i = 0; i < cnt; ++i) {
                if (pr[i] < smallest) {
                    smallest   = pr[i];
                    exp_small  = ex[i];
                }
                if (pr[i] > largest) {
                    largest    = pr[i];
                    exp_large  = ex[i];
                }
            }

            /* compute powers and compare */
            seq_t p_power = big_pow((seq_t)smallest, (seq_t)exp_small);
            seq_t q_power = big_pow((seq_t)largest,  (seq_t)exp_large);

            if (p_power > q_power) {         /* condition satisfied */
                cache[generated++] = (seq_t)term;
            }
        }

        ++current_val;                      /* advance to next integer */
    }

    return cache[n - 1];                     /* 1‑based indexing */
}
