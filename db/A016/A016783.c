/*
 * A016783  a(n) = (3*n+1)^7.
 * https://oeis.org/A016783
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n+1)^7
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 7, u64 n <= 188, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn   = (seq_t)n;
    seq_t base = tn * 3 + 1;
    seq_t r    = base * base * base * base * base * base * base;
    return r;
}
