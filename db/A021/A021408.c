/*
 * A021408  Decimal expansion of 1/404.
 * https://oeis.org/A021408
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(0)=0, a(1)=0; for n>=2, a(n)= [2,4,7,5][(n-2)%4]
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* initial zeros */
    if (n < 2)
        return 0;

    /* periodic pattern starting at n=2 */
    static const seq_t pattern[4] = {2, 4, 7, 5};
    return pattern[(n - 2) % 4];
}
