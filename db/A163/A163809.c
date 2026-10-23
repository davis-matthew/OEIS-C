/*
 * A163809  Write n in binary. Insert a 0 in the middle of each pair of two
 *          consecutive 1's. a(n) = the decimal value of the result.
 * https://oeis.org/A163809
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the decimal value of the binary representation of n after
 *            inserting a 0 between every pair of consecutive 1's
 * Range:     u8 n <= 30, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find the index of the most significant 1‑bit (0‑based). */
    int msb = 63;
    while ((n >> msb) == 0)
        --msb;                 /* n >= 1, so msb will be in [0,63] */

    seq_t out = 0;             /* accumulated output bits */
    int prev = 0;              /* previous input bit */

    for (int i = msb; i >= 0; --i) {
        int bit = (int)((n >> i) & 1ull);

        if (i == msb) {         /* the leading 1 – just copy it */
            out = bit;
        } else {
            if (bit && prev)    /* consecutive 1’s: insert a 0 */
                out = (out << 2) | 1;
            else                /* normal append */
                out = (out << 1) | bit;
        }
        prev = bit;
    }
    return out;
}
