/*
 * A183131  a(n) = 3^(n^2+n) * C(n-1 + 1/3^n, n).
 * https://oeis.org/A183131
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3^(n^2+n) * C(n-1 + 1/3^n, n) a(n) can be written as a(n)
 *            = 3^n * Π_{k=0}^{n-1} (k·3^n + 1) / n! The following
 *            implementation evaluates this product directly
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* Compute M = 3^n */
    seq_t M = (seq_t)1;
    for (idx_t i = 0; i < n; ++i) M *= 3;

    seq_t prod = (seq_t)1;   /* product Π_{k=0}^{n-1} (k·M + 1)   */
    seq_t fact = (seq_t)1;   /* factorial k! for k = 1 .. n-1   */

    for (idx_t k = 1; k < n; ++k)
    {
        seq_t term = M * (seq_t)k + 1;
        prod *= term;
        fact *= (seq_t)k;
    }

    fact *= (seq_t)n;          /* complete the factorial: n!          */
    prod *= M;                 /* multiply by 3^n                     */

    return prod / fact;        /* exact division: result is integer   */
}
