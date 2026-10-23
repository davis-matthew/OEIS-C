/*
 * A021375  Decimal expansion of 1/371.
 * https://oeis.org/A021375
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute decimal expansion of 1/371 by long division */
/* a(n) is the nth digit after the decimal point in 1/371 */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* remainder after the leading 0. (initially 1 for 1/371) */
    uint64_t rem = 1;
    seq_t digit = 0;

    /* perform long division n+1 times to reach digit n */
    for (idx_t i = 0; i <= n; ++i)
    {
        rem = rem * 10ULL;
        digit = (seq_t)(rem / 371ULL);
        rem = rem % 371ULL;
    }
    return digit;
}
