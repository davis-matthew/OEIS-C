/*
 * A016910  a(n) = (6*n)^2.
 * https://oeis.org/A016910
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6*n)^2
 * Range:     u8 n <= 2, u16 n <= 42, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t fn = (seq_t)n;
    return fn * fn * ((seq_t)36);
}
