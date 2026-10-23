/*
 * A166358  Row sums of exponential Riordan array [1+x*arctanh(x), x],
 *          A166357.
 * https://oeis.org/A166358
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 + Σ_{t=1..⌊n/2⌋} binom(n, 2t) * (2t)!/(2t-1)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;   /* a(0) = 1 */

    seq_t result = 1;          /* term for k = 0 (even) */
    seq_t fact   = 1;          /* running factorial, will be k!   */
    seq_t binom  = 1;          /* running binomial coefficient C(n, k) */

    for (idx_t k = 1; k <= n; ++k)
    {
        /* update factorial to k! */
        fact = fact * (seq_t)k;

        /* update binomial coefficient to C(n, k) */
        binom = binom * (seq_t)(n - k + 1);
        binom = binom / (seq_t)k;      /* exact division */

        if ((k & 1) == 0)              /* even k only contribute */
        {
            /* (k)!/(k-1) is an integer */
            seq_t fact_div = fact / (seq_t)(k - 1);

            /* term = C(n, k) * (k)!/(k-1) */
            seq_t term = binom * fact_div;
            result += term;
        }
    }

    return result;
}
