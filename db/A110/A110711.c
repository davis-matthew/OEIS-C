/*
 * A110711  Number of linear arrangements of n blue, n red and n green items
 *          such that first and last elements have the same color but there
 *          are no adjacent items of the same color.
 * https://oeis.org/A110711
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=0, a(2)=6; For n>=3:
 *            a(n)=((7*n^2-13*n+4)*a(n-1)+8*(n-2)^2*a(n-2))/((n+1)*(n-2))
 *            (Division is exact for all integer n>=3)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1) return 0;
    if (n == 2) return 6;

    /* Keep two previous terms */
    seq_t a_prev2 = 0;  /* a(1) */
    seq_t a_prev1 = 6;  /* a(2) */
    seq_t a_cur   = 0;  /* a(n) */

    for (idx_t i = 3; i <= n; ++i)
    {
        seq_t coeff1   = (seq_t)(7 * i * i - 13 * i + 4);
        seq_t coeff2   = (seq_t)(8 * (i - 2) * (i - 2));
        seq_t numerator   = coeff1 * a_prev1 + coeff2 * a_prev2;
        seq_t denominator = (seq_t)(i + 1) * (i - 2);
        a_cur = numerator / denominator;  /* exact division */

        /* Slide the window */
        a_prev2 = a_prev1;
        a_prev1 = a_cur;
    }

    return a_cur;
}
