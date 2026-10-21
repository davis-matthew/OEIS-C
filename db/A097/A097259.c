/*
 * A097259  Numbers whose set of base 13 digits is {0,C}, where C base 13 = 12
 *          base 10.
 * https://oeis.org/A097259
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 12*A033049(n) a(0) = 0 Recurrence: a(2n) = 13*a(n)
 *            a(2n+1) = a(2n) + 12 Since A033049(n) are the numbers having
 *            only 0 or 1 as base‑13 digits, we can compute a(n) by
 *            interpreting the binary representation of `n` and building the
 *            base‑13 number with digits 0 or 12
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 255, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* for uint64_t etc. */

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    seq_t result = 0;

    /* Find the position of the most significant bit of n */
    idx_t msb = 0;
    idx_t tmp = n;
    while (tmp >>= 1) msb++;

    /* Build the number from the most significant bit downwards */
    for (idx_t i = msb; ; --i) {
        result = result * 13;                 /* shift left by one base‑13 digit */
        if (n & ((idx_t)1 << i))             /* if the i‑th bit is 1 */
            result += 12;                    /* add the digit 12 (C in base‑13) */

        if (i == 0) break;                    /* finished all bits */
    }

    return result;
}
