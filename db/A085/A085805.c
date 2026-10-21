/*
 * A085805  Numbers k such that the permanent of the character table of the
 *          dihedral group D_k is not zero.
 * https://oeis.org/A085805
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 16*(n-1)+4 (n ≥ 1)
 * Range:     u8 n <= 7, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is at least 1 (offset=1) */
    seq_t n1 = (seq_t)(n - 1);
    return 16 * n1 + 4;
}
