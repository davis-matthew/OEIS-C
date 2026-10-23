/*
 * A192884  Non-superabundant numbers satisfying the reverse of Robin's
 *          inequality (A091901).
 * https://oeis.org/A192884
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th non‑superabundant number that satisfies Robin's
 *            reverse inequality: σ(m) > e^γ·m·log(log(m))
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This implementation iterates over integers m ≥ 3, computes
    σ(m) via prime factorisation, checks the inequality, keeps the
    running maximum of σ(m)/m using exact integer arithmetic,
    and stores all qualifying numbers.  The search is memoised across
    calls so any order of calls yields O(a(n)) work. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/factor_u64.h"

/* not redefined: seq_t, idx_t                                                                 */
seq_t a(idx_t n)
{
    /* static state shared between calls --------------------------------- */
    static seq_t *cache = NULL;      /* stored qualifying numbers      */
    static size_t cache_len = 0;     /* current number of entries     */
    static size_t cache_cap = 0;     /* allocated capacity (in seq_t) */
    static idx_t next_m = 3;         /* next integer to test          */

    /* maximum ratio recorded so far as σ/max_num (exact integers) */
    static seq_t max_sigma = (seq_t)3;   /* σ(2) = 3                       */
    static seq_t max_num   = (seq_t)2;   /* the n for max_sigma            */

    /* expand the cache until it contains the requested term --------- */
    while (cache_len < (size_t)n) {
        /* ---------- compute σ(next_m) -------------------------------- */
        uint64_t primes[64];
        uint64_t exps[64];
        size_t   cnt = 0;
        factor_u64((uint64_t)next_m, primes, exps, &cnt);

        __uint128_t sigma = 1;
        for (size_t i = 0; i < cnt; ++i) {
            uint64_t p = primes[i];
            uint64_t e = exps[i];
            /* compute (p^(e+1)-1)/(p-1) using 128‑bit arithmetic */
            __uint128_t pow = 1;
            for (uint64_t j = 0; j < e + 1; ++j)
                pow *= p;
            pow -= 1;
            pow /= (p - 1);
            sigma *= pow;
        }

        /* ---------- reverse Robin inequality: σ > e^γ·n·log(log(n)) ----- */
        long double rhs = 1.781072417990197L *
                          (long double)next_m *
                          logl(logl((long double)next_m));

        if ((long double)sigma > rhs) {
            /* ---------- check for superabundance ----------------------- */
            /* compare σ/next_m with current maximum ratio            */
            seq_t left  = (seq_t)sigma;
            left *= max_num;               /* left  = σ * max_num            */
            seq_t right = max_sigma;
            right *= next_m;                /* right = max_sigma * next_m     */

            if (left > right) {            /* superabundant: update max       */
                max_sigma = (seq_t)sigma;
                max_num   = (seq_t)next_m;
            } else {                       /* non‑superabundant: store in cache */
                if (cache_len == cache_cap) {
                    cache_cap = cache_cap ? cache_cap * 2 : 8;
                    cache = (seq_t *)realloc(cache,
                                              cache_cap * sizeof(seq_t));
                }
                cache[cache_len++] = (seq_t)next_m;
            }
        }
        ++next_m;                           /* next candidate integer        */
    }
    return cache[n - 1];                    /* indices are 1‑based           */
}
