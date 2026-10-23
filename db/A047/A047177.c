/*
 * A047177  Number of nonempty subsets of {1,2,...,n} in which exactly 2/5 of
 *          the elements are <= (n-1)/2.
 * https://oeis.org/A047177
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=1}^{min(floor(n/5), floor(L/2), floor(R/3))}
 *            C(L,2k)*C(R,3k) where L = floor((n-1)/2) and R = n – L
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Helper: compute binomial coefficient C(N,K) as seq_t. */
static seq_t binom(idx_t N, idx_t K)
{
    if (K > N) return 0;
    if (K == 0 || K == N) return 1;
    if (K > N - K) K = N - K;
    seq_t res = 1;
    for (idx_t i = 1; i <= K; ++i)
    {
        seq_t num = (seq_t)(N - K + i);
        seq_t den = (seq_t)i;
        res = (res * num) / den;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* lower and upper halves */
    idx_t L = (n - 1) / 2;
    idx_t R = n - L;

    /* maximum k allowed by the size constraints */
    idx_t maxk = n / 5;
    if (L / 2 < maxk) maxk = L / 2;
    if (R / 3 < maxk) maxk = R / 3;

    seq_t total = 0;
    for (idx_t k = 1; k <= maxk; ++k)
    {
        seq_t lower = binom(L, 2 * k);
        seq_t upper = binom(R, 3 * k);
        total += lower * upper;
    }
    return total;
}
