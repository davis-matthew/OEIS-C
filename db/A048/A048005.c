/*
 * A048005  Number of nonempty subsets of {1,2,...,n} in which exactly 1/2 of
 *          the elements are <= (n-1)/3.
 * https://oeis.org/A048005
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=1}^{min(m, n-m)} C(m,k) * C(n-m,k), where m =
 *            ⌊(n-1)/3⌋ and C(a,b) is the binomial coefficient
 * Range:     u8 n <= 12, u16 n <= 21, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* m = floor((n-1)/3) */
    idx_t m  = (n - 1) / 3;
    idx_t mm = n - m;                     /* n - m = elements > m */
    idx_t kmax = (m < mm) ? m : mm;        /* up to min(m, n-m) */
    if (kmax == 0)
        return (seq_t)0;                   /* no subset can satisfy the condition */

    seq_t sum   = 0;
    seq_t binom1 = m;                     /* C(m,1) */
    seq_t binom2 = mm;                    /* C(n-m,1) */

    for (idx_t k = 1; k <= kmax; ++k) {
        if (k > 1) {
            /* C(m,k) = C(m,k-1) * (m-k+1) / k */
            binom1 = binom1 * (seq_t)(m - k + 1) / (seq_t)k;
            /* C(n-m,k) = C(n-m,k-1) * (n-m-k+1) / k */
            binom2 = binom2 * (seq_t)(mm - k + 1) / (seq_t)k;
        }
        sum += binom1 * binom2;
    }
    return sum;
}
