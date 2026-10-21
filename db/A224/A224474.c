/*
 * A224474  (2*16^(5^n) - 1) mod 10^n: a sequence of trimorphic numbers ending
 *          in 1.
 * https://oeis.org/A224474
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (2 * 16^(5^n) - 1) mod 10^n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   mul_small, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A224474 – trimorphic numbers ending in 1. */
/* The function builds terms incrementally, so calls with any
   ordering of indices work correctly. */

/* --------------------------------------------------------- */
/* Required utilities:                                        */
/* --------------------------------------------------------- */
#include <stddef.h>
#include <stdlib.h>
#include "../utils/mul_small.h"          /* mul_small(x, m)   */
#include "../utils/pow10.h"             /* pow10(exp)         */

/* --------------------------------------------------------- */
/* Main sequence function                                     */
/* --------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Static cache to store previously computed values */
    static seq_t *cache        = NULL;    /* array indexed by n  */
    static idx_t  cache_size   = 0;       /* allocated size     */
    static idx_t  last_n       = 0;       /* largest n computed */

    /* Pre‑computed powers for the previous index */
    static seq_t pow2_curr   = 0;         /* 2^last_n            */
    static seq_t pow5_curr   = 0;         /* 5^last_n            */
    static seq_t pow10_curr  = 0;         /* 10^last_n           */

    /* 2^last_n mod 5 – needed for the CRT step */
    static unsigned two_mod5 = 0;          /* value in {0..4}     */

    /* --- Initialization ----------------------------------- */
    if (cache == NULL) {
        cache_size = 2;                    /* indices 0 .. 1      */
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        cache[0] = 0;   /* unused placeholder */
        cache[1] = 1;   /* a(1) = 1           */

        /* initialise for last_n = 1 */
        last_n   = 1;
        pow2_curr   = 2;       /* 2^1                */
        pow5_curr   = 5;       /* 5^1                */
        pow10_curr  = 10;      /* 10^1               */
        two_mod5    = 2 % 5;   /* 2^1 mod 5 = 2      */
    }

    /* --- Expand cache if the requested index is larger ---- */
    if (n > last_n) {
        /* Allocate new size */
        size_t new_size = (size_t)n + 1;
        if (new_size > cache_size) {
            cache = (seq_t *)realloc(cache,
                                     sizeof(seq_t) * new_size);
            cache_size = new_size;
        }

        /* Precomputed inverse table for 1..4 modulo 5 */
        const unsigned inv_two_mod5[5] = {0, 1, 3, 2, 4};

        /* Generate terms up to n */
        for (idx_t cur = last_n + 1; cur <= n; ++cur) {
            seq_t a_prev = cache[cur - 1];

            /* s = (a_prev + 1) / 2^last_n  (exact) */
            seq_t s = (a_prev + 1) / pow2_curr;
            int parity = (int)(s % 2);            /* t ≡ s (mod 2) */

            /* m = (a_prev - 1) / 5^last_n  (exact) */
            seq_t m = (a_prev - 1) / pow5_curr;
            int m_mod5 = (int)(m % 5);            /* needed for t ≡ c (mod 5) */

            /* Compute t modulo 5 using CRT */
            int inv = inv_two_mod5[two_mod5];
            int c = ((5 - m_mod5) % 5 * inv) % 5;  /* t ≡ c (mod 5) */

            /* Find t in 0..9 satisfying both congruences */
            int t = -1;
            for (int cand = 0; cand < 10; ++cand) {
                if ((cand % 2 == parity) && (cand % 5 == c)) {
                    t = cand;
                    break;
                }
            }
            /* t must always be found */
            if (t < 0) t = 0;

            /* New term: a_new = a_prev + t * 10^last_n */
            seq_t term   = mul_small(pow10_curr, (unsigned)t);
            seq_t a_new  = a_prev + term;
            cache[cur] = a_new;

            /* Update powers for next iteration */
            pow2_curr   = mul_small(pow2_curr, 2);
            pow5_curr   = mul_small(pow5_curr, 5);
            pow10_curr  = mul_small(pow10_curr, 10);
            two_mod5    = (two_mod5 * 2) % 5;
        }
        last_n = n;
    }

    return cache[n];
}
