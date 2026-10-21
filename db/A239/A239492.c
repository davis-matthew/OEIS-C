/*
 * A239492  The fifth bicycle lock sequence: a(n) is the maximum value of
 *          min{x*y, (5-x)*(n-y)} over 0 <= x <= 5, 0 <= y <= n for integers
 *          x, y.
 * https://oeis.org/A239492
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor(n/5) - [n == 1 mod 5]
 * Range:     u8 n <= 213, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = (seq_t)n + (seq_t)(n / 5); /* n + floor(n/5) */
    if (n % 5 == 1)                           /* subtract 1 if n ≡ 1 (mod 5) */
        result -= 1;
    return result;
}
