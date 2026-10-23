/*
 * A014494  Even triangular numbers.
 * https://oeis.org/A014494
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = (2*n+1)*(2*n+1-(-1)^n)/2
 * Range:     u8 n <= 10, u16 n <= 180, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t t = (seq_t)(2*n + 1);
    seq_t inner = (n & 1) ? t + (seq_t)1 : t - (seq_t)1;
    return (t * inner) / (seq_t)2;
}
