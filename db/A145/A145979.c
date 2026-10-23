/*
 * A145979  a(n) = (2*n + 4)/gcd(n,4).
 * https://oeis.org/A145979
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n + 4)/gcd(n,4)
 * Range:     u8 n <= 125, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the numerator (2*n + 4) in seq_t to avoid overflow. */
    seq_t numerator = (seq_t)n * 2 + 4;

    /* Determine gcd(n,4) by inspecting n modulo 4. */
    seq_t g;
    if ((n & 3) == 0)          /* n divisible by 4 */
        g = 4;
    else if ((n & 1) == 0)     /* n even but not divisible by 4 */
        g = 2;
    else                       /* n odd */
        g = 1;

    return numerator / g;
}
