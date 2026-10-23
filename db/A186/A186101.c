/*
 * A186101  a(n) = 2*n / 3 if n divisible by 3, a(n) = n otherwise.
 * https://oeis.org/A186101
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = 2*n/3 if n % 3 == 0, otherwise a(n) = n
 * Range:     u8 n <= 255, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    if ((x % (seq_t)3) == 0)
        return (x * 2) / 3;
    else
        return x;
}
