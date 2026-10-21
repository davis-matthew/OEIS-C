/*
 * A072702  Last digit of F(n) is 4 where F(n) is the n-th Fibonacci number.
 * https://oeis.org/A072702
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 60 * floor((n - 1) / 4) + [9, 12, 18, 51][(n - 1) % 4]
 * Range:     u8 n <= 18, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t offsets[4] = {9, 12, 18, 51};
    idx_t cycles = (n - 1) / 4;
    idx_t rem    = (n - 1) % 4;
    return (seq_t)cycles * 60 + offsets[(unsigned)rem];
}
