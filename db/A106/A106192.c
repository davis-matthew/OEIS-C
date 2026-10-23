/*
 * A106192  Expansion of sqrt(1-4x)/(1-x^2).
 * https://oeis.org/A106192
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   n*a(n) + 2*(-2*n+3)*a(n-1) - n*a(n-2) + 2*(2*n-3)*a(n-3) = 0
 *            Initial values: a(0)=1, a(1)=-2, a(2)=-1
 * Range:     i8 n <= 6, i16 n <= 10, i32 n <= 17, i64 n <= 26, i128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return -2;
    if (n == 2) return -1;

    /* a(0), a(1), a(2) */
    seq_t prev3 = 1;   /* a(n-3) when n===3 */
    seq_t prev2 = -2;  /* a(n-2) when n===3 */
    seq_t prev1 = -1;  /* a(n-1) when n===3 */
    seq_t curr;

    for (idx_t k = 3; k <= n; ++k) {
        /* factor = 4*k - 6, computed as seq_t to avoid overflow */
        seq_t factor = (seq_t)k * 4 - (seq_t)6;

        curr = (factor * prev1 + (seq_t)k * prev2 - factor * prev3) / (seq_t)k;

        /* shift positions for next iteration */
        prev3 = prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
