/*
 * A070577  a(n) = n^4 mod 34.
 * https://oeis.org/A070577
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^4 mod 34
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Reduce n modulo 34 first to keep intermediate values tiny. */
    seq_t x = (seq_t)n % 34;
    /* Compute x^2 mod 34 and then square again to get x^4 mod 34. */
    x = (x * x) % 34;
    x = (x * x) % 34;
    return x;
}
