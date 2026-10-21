/*
 * A073015  a(n) is such that 2 = sqrt(1+sqrt(1+sqrt(1+....sqrt(a(n))....)))
 *          where there are n sqrt's.
 * https://oeis.org/A073015
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(n) = 1 + x_n where x_0 = 2 and x_{k+1} = x_k^2 - 1.
 *            Equivalently, a(n) = 1 + ((...(2^2-1)^2-1)... +1) taken n times
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t v = 2;
    for (idx_t i = 0; i < n; ++i)
        v = v * v - 1;
    return v + 1;
}
