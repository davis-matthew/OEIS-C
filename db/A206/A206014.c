/*
 * A206014  Number of (n+1) X 2 0..3 arrays with the number of clockwise edge
 *          increases in every 2 X 2 subblock unequal to the number of
 *          counterclockwise edge increases.
 * https://oeis.org/A206014
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) + 10*a(n-2), a(1)=104, a(2)=680
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)104;
    if (n == 2)
        return (seq_t)680;

    seq_t prev2 = (seq_t)104;
    seq_t prev1 = (seq_t)680;
    seq_t cur;

    for (idx_t i = 3; i <= n; ++i) {
        cur = prev1 * (seq_t)5 + prev2 * (seq_t)10;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
