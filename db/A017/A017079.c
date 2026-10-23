/*
 * A017079  a(n) = (8*n + 1)^3.
 * https://oeis.org/A017079
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*n + 1)^3
 * Range:     u8 n <= 0, u16 n <= 4, u32 n <= 203, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t t = ((seq_t)n * (seq_t)8) + (seq_t)1;
    seq_t t2 = t * t;
    return t2 * t;
}
