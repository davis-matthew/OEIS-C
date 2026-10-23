/*
 * A017118  a(n) = (8*n + 4)^6.
 * https://oeis.org/A017118
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*n + 4)^6
 * Range:     u8 none, u16 n <= 0, u32 n <= 4, u64 n <= 202, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base   = (seq_t)8 * (seq_t)n + (seq_t)4;
    seq_t result = base * base * base * base * base * base;
    return result;
}
