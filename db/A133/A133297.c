/*
 * A133297  a(n) = n! * Sum_{k=1..n} (-1)^(k + 1) * n^(n - k - 1)/(n - k)!.
 * https://oeis.org/A133297
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A133297
   Formula: a(n) = n! * Σ_{k=1..n} (-1)^(k+1) * n^(n-k-1) / (n-k)!
   Equivalent iterative form:
     Compute f_1 = n^(n-1),
     then for k = 1..n:
         add f_k to the sum with sign (-1)^(k+1),
         f_{k+1} = f_k * (n-k) / n     (exact division)
   Finally a(n) = Σ_{k odd} f_k - Σ_{k even} f_k.
*/

#include <stdbool.h>

seq_t a(idx_t n) {
    /* a(0) = 0 */
    if (n == 0) return (seq_t)0;

    /* f = n^(n-1) */
    seq_t f = (seq_t)1;
    for (idx_t i = 1; i <= n - 1; ++i) {
        f *= (seq_t)n;
    }

    seq_t posSum = (seq_t)0, negSum = (seq_t)0;

    for (idx_t k = 1; k <= n; ++k) {
        /* sign = (-1)^(k+1) : positive when k is odd */
        if (k & 1) {
            posSum += f;
        } else {
            negSum += f;
        }

        /* update f for next k, except after last iteration */
        if (k < n) {
            f *= (seq_t)(n - k);
            f /= (seq_t)n;   /* exact division */
        }
    }

    /* result = posSum - negSum (posSum ≥ negSum) */
    seq_t result = posSum;
    result -= negSum;
    return result;
}
