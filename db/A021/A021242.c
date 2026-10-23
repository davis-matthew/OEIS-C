/*
 * A021242  Decimal expansion of 1/238.
 * https://oeis.org/A021242
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) is the n‑th digit after the decimal point of 1/238
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t rem = 1;   /* current remainder of 1 divided by 238 */
    seq_t digit = 0;    /* will hold the digit at position n */

    /* compute digits 0 … n */
    for (idx_t i = 0; i <= n; ++i) {
        uint64_t prod = rem * 10;           /* multiply remainder by 10 */
        digit = (seq_t)(prod / 238);        /* next decimal digit */
        rem   = prod % 238;                 /* new remainder for next step */
    }
    return digit;
}
