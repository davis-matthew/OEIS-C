/*
 * A219563  Sum(binomial(n+k,k)^5, k=0..n).
 * https://oeis.org/A219563
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n+k,k)^5
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t bin = 1;          /* bin = binomial(n+k, k), starting with k = 0 */

    /* k = 0 contributes 1^5 = 1 */
    sum = 1;

    for (idx_t k = 1; k <= n; ++k)
    {
        /* Update binomial via recurrence:
           C(n+k, k) = C(n+(k-1), (k-1)) * (n + k) / k */
        bin = (bin * (seq_t)(n + k)) / (seq_t)k;

        /* Compute bin^5 with minimal multiplications */
        seq_t p   = bin * bin;          /* bin^2 */
        seq_t p2  = p * p;              /* bin^4 */
        seq_t pow5 = p2 * bin;          /* bin^5 */

        sum += pow5;
    }

    return sum;
}
