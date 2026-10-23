/*
 * A021553  Decimal expansion of 1/549.
 * https://oeis.org/A021553
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = the (n+1)-th decimal digit of 1/549 (decimal expansion of
 *            1/549 is 0.0018214936...)
 * Range:     u8 n <= 0, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t r = 1;        /* initial remainder */
    seq_t d = 0;
    const seq_t denom = (seq_t)549U;

    /* Perform n+1 steps of long division to reach a(n) */
    for (idx_t i = 0; i <= n; ++i) {
        r *= 10U;
        d = r / denom;
        r = r % denom;
    }
    return d;
}
