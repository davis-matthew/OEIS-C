/*
 * A133653  A007318^(-1) * A003261.
 * https://oeis.org/A133653
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1, a(n)=4*n-2 for n>1
 * Range:     u8 n <= 39, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n <= 1) return 1;
    return ((seq_t)n * 4) - 2;
}
