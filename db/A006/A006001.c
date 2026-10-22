/*
 * A006001  Number of paraffins.
 * https://oeis.org/A006001
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + 5*n/2 + n^3/2
 * Range:     u8 n <= 6, u16 n <= 35, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    seq_t n3 = nn * nn * nn;
    return (2 + 5 * nn + n3) / 2;
}
