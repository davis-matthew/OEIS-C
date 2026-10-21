/*
 * A210440  a(n) = 2*n*(n+1)*(n+2)/3.
 * https://oeis.org/A210440
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n*(n+1)*(n+2)/3
 * Range:     u8 n <= 4, u16 n <= 31, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    seq_t t = x * (x + 1);
    t = t * (x + 2);
    t = t * 2;
    return t / 3;
}
