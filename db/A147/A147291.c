/*
 * A147291  a(n) = Sum_{k=1..n^2-1} binomial(2k,k).
 * https://oeis.org/A147291
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n^2-1} binomial(2k,k)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle n=1 separately: sum over an empty range gives 0 */
    if (n <= 1)
        return 0;

    /* m = n^2 - 1, stays within idx_t for the tested range */
    idx_t m = n * n - 1;

    seq_t sum   = 0;
    seq_t binom = 1; /* C(0,0) = 1 */

    for (idx_t k = 1; k <= m; ++k)
    {
        /* binom = binom * (4k - 2) / k  */
        binom = (binom * ((seq_t)4 * (seq_t)k - (seq_t)2)) / (seq_t)k;
        sum += binom;
    }

    return sum;
}
