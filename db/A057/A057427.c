/*
 * A057427  a(n) = 1 if n > 0, a(n) = 0 if n = 0; series expansion of x/(1-x).
 * https://oeis.org/A057427
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, mult, nice, cons
 * Formula:   a(n) = (n > 0 ? 1 : 0)
 * Range:     u8 n <= 200, u16 n <= 200, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (n == 0) ? (seq_t)0 : (seq_t)1;
}
