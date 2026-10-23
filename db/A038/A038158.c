/*
 * A038158  a(n) = (n!/2)*Sum(1/k!, k=1..n-2).
 * https://oeis.org/A038158
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! / 2 * Sum_{k=1}^{n-2} 1/k! = n*a(n-1) + (n-1)*n/2 (n ≥
 *            3)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n = 0, 1, 2 the sum is empty, hence a(n) = 0. */
    if (n < 3U) return (seq_t)0;

    seq_t res = (seq_t)0;
    for (idx_t i = 3U; i <= n; ++i)
    {
        /* Compute a(i) from a(i-1):
           a(i) = i * a(i-1) + (i-1)*i/2. */
        seq_t prod = (seq_t)i * res;                     /* i * a(i-1) */
        seq_t term = ((seq_t)(i - 1U) * i) / 2U;          /* (i-1)*i/2   */
        res = prod + term;                                /* a(i)        */
    }
    return res;
}
