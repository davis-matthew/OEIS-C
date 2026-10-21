/*
 * A203240  Real part of even numbered terms of the sequence s(n)=(n-1)-st
 *          elementary symmetric function of (i, 2i, 3i,...,ni).
 * https://oeis.org/A203240
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (-1)^(n+1) * (2*n-1)! * Sum_{i=1}^{2*n-1} 1/i
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 6, i64 n <= 10, i128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1; n=0 is undefined */
    idx_t m = 2*n - 1;              /* m = 2*n - 1  (>=1 when n>=1) */

    /* Compute (2*n-1)! */
    seq_t fact = 1;
    for (idx_t i = 2; i <= m; ++i)
        fact *= (seq_t)i;

    /* Sum of fact / i for i = 1..m  (exact integer division) */
    seq_t sum = 0;
    for (idx_t i = 1; i <= m; ++i)
        sum += fact / (seq_t)i;

    /* Apply the sign (-1)^(n+1) */
    if ((n + 1) & 1)   /* odd exponent => sign -1 */
        return -sum;
    else
        return sum;
}
