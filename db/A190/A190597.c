/*
 * A190597  Maximal digit in base-8 expansion of n.
 * https://oeis.org/A190597
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = maximum of the octal digits of n
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t maxd = 0;
    idx_t  temp = n;
    while (temp != 0) {
        seq_t d = temp & 7;    /* least significant base‑8 digit */
        if (d > maxd) {
            maxd = d;
            if (maxd == 7) break;   /* 7 is the largest possible digit */
        }
        temp >>= 3;            /* divide by 8 */
    }
    return maxd;
}
