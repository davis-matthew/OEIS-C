/*
 * A162316  a(n) = 5*n^2 + 20*n + 1.
 * https://oeis.org/A162316
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 5*n^2 + 20*n + 1
 * Range:     u8 n <= 5, u16 n <= 112, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t t = (seq_t)n;
    return t * (5 * t + 20) + 1;
}
