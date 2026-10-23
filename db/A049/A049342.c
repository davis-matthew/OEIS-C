/*
 * A049342  a(n) = A049341(n)/3.
 * https://oeis.org/A049342
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3 - (Fibonacci(n+6) mod 3) – periodic with period 8:
 *            [1,2,3,2,2,1,3,1]
 * Range:     u8 n <= 92, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t pat[8] = {1, 2, 3, 2, 2, 1, 3, 1};
    return pat[n & 7];   /* n % 8 */
}
