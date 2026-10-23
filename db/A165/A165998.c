/*
 * A165998  Denominators of Taylor series expansion of
 *          1/(3*x)*log((1+x)/(1-x)^2).
 * https://oeis.org/A165998
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  frac, nonn, easy
 * Formula:   a(n) = (n+1) if n is even, and 3*(n+1) if n is odd
 *            (equivalently, a(n) = (2-(-1)^n)*(n+1) )
 * Range:     u8 n <= 84, u16 n <= 1004, u32 n <= 1004, u64 n <= 1004,
 *            u128 n <= 1004, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n + 1;            /* n+1 */
    seq_t mult = (n & 1) ? (seq_t)3 : (seq_t)1; /* 3 if n is odd, 1 if even */
    return x * mult;
}
