/*
 * A053438  Expansion of (1+x+2*x^3)/((1-x)*(1-x^2)).
 * https://oeis.org/A053438
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n - (1 + (-1)^n)/2 (n ≥ 1), a(0) = 1
 * Range:     u8 n <= 128, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;
    if (n & 1)               /* n is odd:  (-1)^n = -1  -> subtract 0 */
        return (seq_t)n * 2;
    else                     /* n is even: (-1)^n =  1  -> subtract 1 */
        return (seq_t)n * 2 - 1;
}
