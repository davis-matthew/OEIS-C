/*
 * A154811  a(n) = Fibonacci(2*n+1) mod 9.
 * https://oeis.org/A154811
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(2*n+1) mod 9 The Fibonacci sequence modulo 9
 *            has Pisano period 24, so a(n) repeats with period 12
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 12‑term table of the sequence */
    static const seq_t table[12] = {
        1, 2, 5, 4, 7, 8,
        8, 7, 4, 5, 2, 1
    };
    /* n is unsigned; modulo is safe */
    return table[n % (idx_t)12];
}
