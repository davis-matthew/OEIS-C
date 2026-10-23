/*
 * A109398  a(n) = (1/n!)*Sum_{k=0..n} (n+k)!.
 * https://oeis.org/A109398
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sum_{k=0..n} product_{i=1..k} (n + i)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent to a(n) = (1/n!) * sum_{k=0..n} (n + k)! */

seq_t a(idx_t n)
{
    seq_t term = 1;   /* term = product_{i=1..k} (n + i), initially k=0 */
    seq_t sum  = 1;   /* sum of terms, starting with k=0 term = 1 */

    for (idx_t k = 1; k <= n; ++k)
    {
        /* multiply by (n + k) to extend the product to next k */
        term *= ((seq_t)n + k);
        sum  += term;
    }
    return sum;
}
