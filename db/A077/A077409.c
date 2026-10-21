/*
 * A077409  Bisection (even part) of Chebyshev sequence with Diophantine
 *          property.
 * https://oeis.org/A077409
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1) - a(n-2) (a(-1)=11, a(0)=7)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)7;
    }
    if (n == 1) {
        return (seq_t)59;           /* 10*7 - 11 = 59 */
    }

    seq_t a_nm2 = (seq_t)7;           /* a(0)   */
    seq_t a_nm1 = (seq_t)59;          /* a(1)   */
    seq_t curr = 0;

    /* Compute iteratively from n = 2 up to the requested n */
    for (idx_t i = 2; i <= n; ++i) {
        curr = (seq_t)10 * a_nm1 - a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = curr;
    }
    return curr;
}
