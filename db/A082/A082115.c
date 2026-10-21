/*
 * A082115  Fibonacci sequence (mod 3).
 * https://oeis.org/A082115
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(n) mod 3; The Fibonacci sequence modulo 3 is
 *            periodic with period 8: (0,1,1,2,0,2,2,1)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t period[8] = {0, 1, 1, 2, 0, 2, 2, 1};
    return period[n & 7];
}
