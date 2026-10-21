/*
 * A059278  G.f. is G(x*(1-x)/(1-2*x)) where G(x) is g.f. for Catalan numbers
 *          A000108.
 * https://oeis.org/A059278
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 30, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(0) = 1, a(1) = 1, a(2) = 3, a(3) = 11,
 * (n+1)*a(n) = 3*(3*n-1)*a(n-1) - 12*(2*n-3)*a(n-2)
 *                + 12*(2*n-5)*a(n-3) - 4*(2*n-7)*a(n-4)   (n ≥ 4)
 * Initial values: a(0)=1, a(1)=1, a(2)=3, a(3)=11
 * The implementation memoises computed values and uses 128‑bit arithmetic
 * for 64‑bit seq_t to avoid overflow in intermediate products.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0; /* number of stored elements */

    /* expand the cache if needed */
    if ((size_t)n + 1 > cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) {          /* allocation failed – give up */
            return 0;
        }
        cache = tmp;

        /* initialise any new entries */
        for (size_t k = cache_size; k < new_size; ++k) {
            if (k == 0) {
                cache[k] = (seq_t)1;               /* a(0) */
            } else if (k == 1) {
                cache[k] = (seq_t)1;               /* a(1) */
            } else if (k == 2) {
                cache[k] = (seq_t)3;               /* a(2) */
            } else if (k == 3) {
                cache[k] = (seq_t)11;              /* a(3) */
            } else { /* k >= 4, compute by recurrence */
                idx_t kk = (idx_t)k;
                if (sizeof(seq_t) <= 8) {          /* 64‑bit seq_t, use 128‑bit limbs */
#if defined(__SIZEOF_INT128__)
                    typedef unsigned __int128 u128;
                    u128 k_u128 = (u128)kk;
                    u128 coeff1 = (u128)3 * ((u128)3 * k_u128 - (u128)1);
                    u128 coeff2 = (u128)12 * ((u128)2 * k_u128 - (u128)3);
                    u128 coeff3 = (u128)12 * ((u128)2 * k_u128 - (u128)5);
                    u128 coeff4 = (u128)4  * ((u128)2 * k_u128 - (u128)7);

                    u128 term1 = (u128)cache[k-1] * coeff1;
                    u128 term2 = (u128)cache[k-2] * coeff2;
                    u128 term3 = (u128)cache[k-3] * coeff3;
                    u128 term4 = (u128)cache[k-4] * coeff4;

                    u128 numer = term1 - term2 + term3 - term4;
                    cache[k] = (seq_t)(numer / ((u128)(kk + 1)));
#else
                    /* fall back to seq_t arithmetic if 128‑bit not available */
                    seq_t coeff1 = (seq_t)3 * ((seq_t)3 * kk - (seq_t)1);
                    seq_t coeff2 = (seq_t)12 * ((seq_t)2 * kk - (seq_t)3);
                    seq_t coeff3 = (seq_t)12 * ((seq_t)2 * kk - (seq_t)5);
                    seq_t coeff4 = (seq_t)4  * ((seq_t)2 * kk - (seq_t)7);
                    seq_t numer = cache[k-1] * coeff1 - cache[k-2] * coeff2
                                + cache[k-3] * coeff3 - cache[k-4] * coeff4;
                    cache[k] = numer / ((seq_t)(kk + 1));
#endif
                } else { /* seq_t is an arbitrary‑precision type – no overflow guard needed */
                    seq_t coeff1 = (seq_t)3 * ((seq_t)3 * kk - (seq_t)1);
                    seq_t coeff2 = (seq_t)12 * ((seq_t)2 * kk - (seq_t)3);
                    seq_t coeff3 = (seq_t)12 * ((seq_t)2 * kk - (seq_t)5);
                    seq_t coeff4 = (seq_t)4  * ((seq_t)2 * kk - (seq_t)7);
                    seq_t numer = cache[k-1] * coeff1
                                 - cache[k-2] * coeff2
                                 + cache[k-3] * coeff3
                                 - cache[k-4] * coeff4;
                    cache[k] = numer / ((seq_t)(kk + 1));
                }
            }
        }
        cache_size = new_size;
    }

    return cache[(size_t)n];
}
