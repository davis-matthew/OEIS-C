/*
 * A230682  O.g.f.: Sum_{n>=0} x^n * Product_{k=1..n} (k^2 + x)/(1 + k^2*x).
 * https://oeis.org/A230682
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in \sum_{m>=0} x^m \prod_{k=1}^m
 *            (k^2+x)/(1+k^2x). Recurrence: let r_0(x)=1, and for k≥1 r_k(x) =
 *            ((k^2+x)/(1+k^2x)) * r_{k-1}(x). Then a(n)=\sum_{k=0}^n
 *            [x^{\,n-k}] r_k(x). The code builds the polynomials r_k one by
 *            one and accumulates the necessary coefficients in an a...
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 14, u128 n <= 23,
 *            big any n
 * Helpers:   alloc_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>      /* for size_t */
#include "../utils/alloc_seq_array.h"   /* alloc_seq_array */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;                      /* a(0)=1 */

    /* arrays of length n+1 */
    seq_t *coeff = alloc_seq_array((size_t)n + 1);
    seq_t *cur   = alloc_seq_array((size_t)n + 1);
    seq_t *next  = alloc_seq_array((size_t)n + 1);

    /* initialise arrays */
    for (size_t i = 0; i <= (size_t)n; ++i) {
        coeff[i] = 0;
        cur[i]   = 0;
    }
    coeff[0] = 1;          /* term for k = 0 (r_0 = 1) */
    cur[0]   = 1;          /* r_0(x) = 1 */

    /* iterate over k = 1 … n */
    for (idx_t k = 1; k <= n; ++k) {
        size_t limit = (size_t)(n - k);          /* maximal degree needed */
        seq_t d = (seq_t)k * (seq_t)k;           /* k^2 */

        /* compute coefficients of r_k(x) up to degree "limit" */
        /* temporary storage for division step */
        next[0] = d * cur[0];                    /* temp[0] = d*cur[0] */
        seq_t prev = next[0];                    /* q[-1] = 0 */

        for (size_t i = 1; i <= limit; ++i) {
            seq_t temp = d * cur[i] + cur[i - 1];
            next[i] = temp - d * prev;            /* q[i] = temp - d*q[i-1] */
            prev = next[i];
        }

        /* add the contribution of x^k * r_k to the overall coeff array */
        for (size_t i = 0; i <= limit; ++i) {
            coeff[(size_t)k + i] += next[i];
        }

        /* prepare for next iteration: cur becomes next up to "limit" */
        for (size_t i = 0; i <= limit; ++i)
            cur[i] = next[i];
        /* cur[i>limit] are unused in subsequent steps */
    }

    seq_t result = coeff[(size_t)n];

    /* free allocated memory */
    /* (alloc_seq_array usually provides a free counterpart named free_seq_array) */
    free(coeff);
    free(cur);
    free(next);

    return result;
}
