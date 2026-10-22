/*
 * A000030  Initial digit of n.
 * https://oeis.org/A000030
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, nice, look
 * Formula:   a(n) = floor(n / 10^floor(log_10(n))), a(0) = 0 (the leading
 *            decimal digit of n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    while (n >= 10)
        n /= 10;
    return (seq_t)n;
}
