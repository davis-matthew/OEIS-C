/*
 * A169598  Numbers that are congruent to {3,18} mod 23.
 * https://oeis.org/A169598
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (46*n + 7*(-1)^n - 27)/4
 * Range:     u8 n <= 5, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t num = 46 * nn;
    if (n & 1)
        num -= 7;   /* (-1)^n = -1 for odd n */
    else
        num += 7;   /* (-1)^n =  1 for even n */
    num -= 27;
    return num / 4;
}
