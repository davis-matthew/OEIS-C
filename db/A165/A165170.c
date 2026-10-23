/*
 * A165170  Number of reduced words of length n in Coxeter group on 38
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^8 = I.
 * https://oeis.org/A165170
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Recurrence derived from the rational generating function
   G(t) = (t^8 + 2t^7 + 2t^6 + 2t^5 + 2t^4 + 2t^3 + 2t^2 + 2t + 1) /
          (1 - 36t - 36t^2 - 36t^3 - 36t^4 - 36t^5 - 36t^6 - 36t^7 + 666t^8).

   Let a(n) be the coefficient of t^n in G(t).  For n ≥ 0 we have

        a(n) = 36*a(n‑1) + 36*a(n‑2) + … + 36*a(n‑7) – 666*a(n‑8) + p_n,

   where p_n is the n‑th coefficient of the numerator polynomial:
        p_0 = 1,
        p_1 = p_2 = … = p_7 = 2,
        p_8 = 1,
        p_n = 0 for n > 8.

   Initial values a(0) … a(7) follow automatically from this
   recurrence (with a(k)=0 for k<0).
*/

#include "../utils/mul_small.h"   /* allows seq_t mul_small(seq_t,x,unsigned int) */

seq_t a(idx_t n)
{
    /* Polynomial coefficients of the numerator (degree 8). */
    static const seq_t P[9] = {
        (seq_t)1, (seq_t)2, (seq_t)2, (seq_t)2,
        (seq_t)2, (seq_t)2, (seq_t)2, (seq_t)2, (seq_t)1
    };

    /* Cached values of the sequence. */
    static seq_t *cache = NULL;
    static idx_t cached_len = 0;   /* number of allocated slots  (≥ next_to_compute) */
    static idx_t next_to_compute = 0;   /* first index not yet computed */

    /* Allocate/resize the cache to hold a(n). */
    if (n >= cached_len) {
        size_t new_len = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (tmp == NULL) {
            /* Allocation failed – return 0 as an error indicator. */
            return (seq_t)0;
        }
        cache = tmp;
        cached_len = (idx_t)new_len;
    }

    /* Compute successive values up to n. */
    for (idx_t i = next_to_compute; i <= n; ++i) {
        /* 36 * sum_{k=1..7} a(i-k) */
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k <= 7 && k <= i; ++k) {
            sum += cache[i - k];
        }
        sum = mul_small(sum, 36u);   /* 36 is small enough for unsigned int */

        seq_t val = sum;

        /* Subtract 666 * a(i-8) if applicable */
        if (i >= 8) {
            seq_t term = mul_small(cache[i - 8], 666u);
            val -= term;
        }

        /* Add the polynomial coefficient if i ≤ 8 */
        if (i <= 8) {
            val += P[i];
        }

        cache[i] = val;
    }

    next_to_compute = n + 1;
    return cache[n];
}
