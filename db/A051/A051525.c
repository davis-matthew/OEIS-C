/*
 * A051525  Third unsigned column of triangle A051338.
 * https://oeis.org/A051525
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Σ_{k=2}^{n} C(n,k)*(k-1)!*H_{k-1}*(6)_{n-k}
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence used:
     a(0)=0, a(1)=0
     t_n = (n-1)!*H_{n-1}
     a_n = t_n + Σ_{k odd} C(6,k)*(n)_k*a_{n-k}
                - Σ_{k even} C(6,k)*(n)_k*a_{n-k}
   where (n)_k = n*(n-1)*…*(n-k+1)   */
#include <stdlib.h>

static const seq_t C6[7] = {1,6,15,20,15,6,1};

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t cur_max = 1;          /* largest n already computed in cache   */
    static seq_t t_prev = 0;           /* t_{cur_max} = (cur_max-1)! * H_{cur_max-1} */
    static seq_t fact_prev = 1;        /* cur_max!                               */
    static seq_t fact_prev_prev = 1;   /* (cur_max-1)!                           */

    /* initial cache allocation */
    if (!cache) {
        cache_len = 2;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)0;
    }

    /* enlarge cache if needed */
    if (n + 1 > cache_len) {
        cache_len = n + 1;
        cache = (seq_t *)realloc(cache, cache_len * sizeof(seq_t));
    }

    /* compute terms up to n if required */
    if (n > cur_max) {
        for (idx_t i = cur_max + 1; i <= n; ++i) {
            /* t_i = (i-1)*t_{i-1} + (i-2)! */
            seq_t t_i = ((seq_t)(i - 1)) * t_prev + fact_prev_prev;

            /* a_i via recurrence */
            seq_t sumOdd  = 0;
            seq_t sumEven = 0;
            seq_t prod    = 1;

            for (idx_t k = 1; k <= 6 && k <= i; ++k) {
                if (k == 1)
                    prod = (seq_t)i;
                else
                    prod *= (seq_t)(i - (k - 1));

                seq_t term = C6[k] * prod * cache[i - k];
                if (k & 1)
                    sumOdd += term;
                else
                    sumEven += term;
            }

            seq_t a_i = t_i + sumOdd - sumEven;
            cache[i] = a_i;

            /* update factorial bookkeeping for next i */
            t_prev = t_i;
            seq_t old_fact_prev = fact_prev;
            fact_prev_prev = fact_prev;
            fact_prev = old_fact_prev * (seq_t)i;
            cur_max = i;
        }
    }

    return cache[n];
}
