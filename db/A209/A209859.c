/*
 * A209859  Rewrite the binary expansion of n from the most significant end, 1
 *          -> 1, 0+1 (one or more zeros followed by one) -> 0, drop the
 *          trailing zeros of the original n.
 * https://oeis.org/A209859
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the integer obtained by rewriting the binary expansion of
 *            n from the most significant end according to: 1 → 1, 0+1 (one or
 *            more zeros followed by a 1) → 0; the trailing zeros of n are
 *            discarded
 * Range:     u8 n <= 108, u16 n <= 108, u32 n <= 108, u64 n <= 108,
 *            u128 n <= 108, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;           /* a(0) = 0 */

    /* Drop the trailing zeros of n.  For n>0 the least significant bit of m is 1. */
    idx_t m = n >> __builtin_ctzll(n);

    /* Position of the most significant set bit in m. */
    int msb = 63 - __builtin_clzll(m);   /* m != 0 */

    idx_t out = 0;

    /* Scan from the most significant bit to the least. */
    for (int i = msb; i >= 0; ) {
        if (m & ((idx_t)1 << i)) {
            /* Current bit is 1 → keep it. */
            out = (out << 1) | 1;
            i--;
        } else {
            /* Current bit is 0: skip the zero run until the next 1, output a single 0. */
            while (i >= 0 && !(m & ((idx_t)1 << i)))
                i--;
            out = (out << 1);   /* write 0 (implicit) */
            i--;                /* skip the 1 that terminates the run */
        }
    }

    return (seq_t)out;
}
