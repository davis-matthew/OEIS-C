/*
 * A085624  Sum of the entries in the character table of the dihedral group
 *          D_{2n} of order 2n.
 * https://oeis.org/A085624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3n + 1)/2 if n is odd; (3n + 2)/2 if n ≡ 2 (mod 4); (3n
 *            + 4)/2 if n is a multiple of 4
 * Range:     u8 n <= 96, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t res = sn * 3;
    if ((n & 1) == 1) {
        res += 1;
    } else if ((n & 2) == 2) {
        res += 2;
    } else {
        res += 4;
    }
    return res / 2;
}
