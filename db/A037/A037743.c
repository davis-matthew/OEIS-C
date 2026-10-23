/*
 * A037743  Decimal expansion of a(n) is given by the first n terms of the
 *          periodic sequence with initial period 2,1,3,0.
 * https://oeis.org/A037743
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 10*a(n-1) + d(n), a(1)=2, d(n) = {2,1,3,0}[(n-1) % 4]
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t period[4] = { 2, 1, 3, 0 };

seq_t a(idx_t n) {
    if (n == 0)
        return 0; /* undefined for n<1, but return 0 */

    /* a(1) = 2 */
    seq_t result = 2;
    if (n == 1)
        return result;

    /* Iterate from 2 to n, building the decimal number digit by digit */
    for (size_t i = 2; i <= (size_t)n; ++i) {
        seq_t digit = period[(i - 1) % 4];
        result = result * 10 + digit;
    }
    return result;
}
