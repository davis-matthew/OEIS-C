/*
 * A008381  floor(n/5)*floor((n+1)/5)*floor((n+2)/5)*floor((n+3)/5).
 * https://oeis.org/A008381
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n/5)*floor((n+1)/5)*floor((n+2)/5)*floor((n+3)/5)
 * Range:     u8 n <= 19, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t f0 = (seq_t)n / 5;
    seq_t f1 = (seq_t)(n + 1) / 5;
    seq_t f2 = (seq_t)(n + 2) / 5;
    seq_t f3 = (seq_t)(n + 3) / 5;
    return f0 * f1 * f2 * f3;
}
