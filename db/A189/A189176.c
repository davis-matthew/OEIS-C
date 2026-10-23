/*
 * A189176  Row sums of the Riordan matrix (1+x/sqrt(1-4*x),(1-sqrt(1-4*x))/2)
 *          (A189175).
 * https://oeis.org/A189176
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS formula:
 *  a(0) = 1
 *  For n ≥ 1:
 *      a(n) = Σ_{k=1..n} (3‑k) * binomial(2*n‑k‑1, n‑1)
 *  This implementation uses the recurrence
 *      B_k = binomial(2n‑k‑1, n‑1)
 *      B_{k+1} = B_k * (n‑k) / (2n‑k‑1)
 *  to achieve O(n) time with only integer arithmetic.
 */
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t sum_pos = 0, sum_neg = 0;
    seq_t bin = binomial(2*n - 2, n - 1);   /* B_1 */

    for (idx_t k = 1; k <= n; ++k) {
        if (k <= 3) {
            sum_pos += bin * (seq_t)(3 - k);   /* positive part  (3‑k) ≥ 0 */
        } else {
            sum_neg += bin * (seq_t)(k - 3);   /* negative part handled separately */
        }

        if (k < n) {   /* update bin to B_{k+1} */
            seq_t numer = bin * (seq_t)(n - k);
            seq_t denom = (seq_t)(2*n - k - 1);
            bin = numer / denom;   /* exact division */
        }
    }
    return sum_pos - sum_neg;   /* sum_pos ≥ sum_neg for all n ≥ 1 */
}
