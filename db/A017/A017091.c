/*
 * A017091  a(n) = (8*n + 2)^3.
 * https://oeis.org/A017091
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*n + 2)^3
 * Range:     u8 n <= 0, u16 n <= 4, u32 n <= 202, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = ((seq_t)8 * (seq_t)n) + 2;
    seq_t sq   = base * base;
    return sq * base;
}
