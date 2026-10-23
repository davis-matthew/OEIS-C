/*
 * A172100  Diagonal of the 26 X 26 Caesar Shift table.
 * https://oeis.org/A172100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less, fini, full
 * Formula:   a(n) = 1 + (2*n mod 26)
 * Range:     u8 n <= 26, u16 n <= 26, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute 2*n modulo 26, then add 1. */
    seq_t val = ((seq_t)n * 2) % 26;
    return val + 1;
}
