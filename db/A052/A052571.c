/*
 * A052571  E.g.f. x^3/(1-x)^2.
 * https://oeis.org/A052571
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 0 for n < 3, otherwise a(n) = (n-2) * n!
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS A052571 */
seq_t a(idx_t n)
{
    /* Treat small indices explicitly */
    if (n < 3)
        return 0;

    /* Compute n! */
    seq_t fact = 1;
    for (idx_t i = 1; i <= n; ++i)
        fact *= (seq_t)i;

    /* Multiply by (n-2) */
    return fact * (seq_t)(n - 2);
}
