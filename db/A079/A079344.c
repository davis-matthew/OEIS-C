/*
 * A079344  F(n) mod 8, where F(n) = A000045(n) is the n-th Fibonacci number.
 * https://oeis.org/A079344
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(n) mod 8, period 12
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Pisano period for modulo 8 is 12 */
    static const seq_t pattern[12] = {
        0, 1, 1, 2, 3, 5, 0, 5, 5, 2, 7, 1
    };
    return pattern[n % 12];
}
