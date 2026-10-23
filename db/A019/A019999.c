/*
 * A019999  Number of similarity classes of descendants created by bisection
 *          refinement from an initial n-simplex.
 * https://oeis.org/A019999
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^3 + 5 n^2 + 8 n + 4) * n! * 2^n, with a(0)=4, a(1)=36,
 *            etc
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t fact = 1;      /* n! */
    seq_t pow2 = 1;      /* 2^n */

    /* Compute factorial and power of two iteratively. */
    for (idx_t i = 2; i <= n; ++i)
        fact *= (seq_t)i;

    for (idx_t i = 1; i <= n; ++i)
        pow2 *= 2;

    /* Polynomial p = n^3 + 5 n^2 + 8 n + 4 */
    seq_t ns = (seq_t)n;
    seq_t p = ns * ns * ns;          /* n^3 */
    p += 5 * ns * ns;                /* 5 n^2 */
    p += 8 * ns;                     /* 8 n */
    p += 4;                          /* constant term */

    /* Final result */
    seq_t result = fact;
    result *= pow2;
    result *= p;
    return result;
}
