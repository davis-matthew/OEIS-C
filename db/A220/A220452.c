/*
 * A220452  Number of unordered full binary trees with labels from a set of n
 *          labels.
 * https://oeis.org/A220452
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..n} binomial(n,k)*(2k-3)!!
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t binom = 1;            /* current binomial coefficient binom(n,k)   */
    seq_t odd   = 1;            /* (2k-3)!! for current k                    */

    for (idx_t k = 1; k <= n; ++k) {
        if (k > 1)
            odd *= (seq_t)(2 * k - 3);

        binom *= (seq_t)(n - k + 1);   /* binom *= (n-k+1) */
        binom /= (seq_t)k;             /* exact division */

        sum += binom * odd;
    }

    return sum;
}
