/*
 * A157014  Expansion of x*(1-x)/(1 - 22*x + x^2).
 * https://oeis.org/A157014
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 22*a(n-1) - a(n-2) * with initial values a(1) = 1, a(2) =
 *            21 * (Offset 1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the first two terms directly */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)21;

    seq_t prev2 = (seq_t)1;   /* a(1) */
    seq_t prev1 = (seq_t)21;  /* a(2) */
    seq_t cur = 0;

    /* iterate from the third term up to n */
    for (idx_t i = 3; i <= n; ++i)
    {
        cur = (seq_t)22 * prev1 - prev2;
        prev2 = prev1;
        prev1 = cur;
    }

    return cur;  /* cur holds a(n) after the loop */
}
