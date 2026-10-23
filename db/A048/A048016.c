/*
 * A048016  Number of nonempty subsets of {1,2,...,n} in which exactly 1/2 of
 *          the elements are <= (n-2)/3.
 * https://oeis.org/A048016
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * Let A = floor((n-2)/3), B = n-A. * a(n) = Σ_{k=1}^{min(A,B)}
 *            binom(A,k) * binom(B,k) * (nonempty subsets of {1..n} with
 *            exactly half of the elements ≤ A) * [The sum runs over
 *            k=1..min(A,B).]
 * Range:     u8 n <= 12, u16 n <= 21, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    idx_t A = (n >= 2) ? (n - 2) / 3 : 0;   /* floor((n-2)/3)  */
    idx_t B = n - A;                        /* remaining elements  */
    idx_t maxk = (A < B) ? A : B;           /* min(A,B)           */
    if (maxk == 0)
        return 0;                           /* no valid k ≥ 1     */

    seq_t sum = 0;
    seq_t combA = 1;   /* binom(A,0) */
    seq_t combB = 1;   /* binom(B,0) */

    for (idx_t k = 1; k <= maxk; ++k)
    {
        /* update combA = binom(A,k) */
        combA = (combA * (seq_t)(A - k + 1)) / k;
        /* update combB = binom(B,k) */
        combB = (combB * (seq_t)(B - k + 1)) / k;
        sum += combA * combB;
    }
    return sum;
}
