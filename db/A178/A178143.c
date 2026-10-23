/*
 * A178143  Sum of squares d^2 over the divisors d=2 and/or d=3 of n.
 * https://oeis.org/A178143
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = sum_{d|n, d=2 and/or d=3} d^2
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 0;
    /* add 4 if n is even (divisible by 2) */
    if ((n & 1) == 0) res += 4;
    /* add 9 if n is divisible by 3 */
    if (n % 3 == 0) res += 9;
    return res;
}
