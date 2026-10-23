/*
 * A123162  Triangle read by rows: T(n,k) = binomial(2*n - 1, 2*k - 1) for 0 <
 *          k <= n and T(n,0) = 1.
 * https://oeis.org/A123162
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   T(n,k) = binomial(2*n-1, 2*k-1) for 0 < k <= n, T(n,0)=1. The
 *            sequence a() is the flattened triangle, indexed from 0
 * Range:     u8 n <= 23, u16 n <= 59, u32 n <= 179, u64 n <= 644,
 *            u128 n <= 1325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t binom(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    unsigned __int128 res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (n - k + i);
        res = res / i;
    }
    return (seq_t)res;
}

seq_t a(idx_t n)
{
    /* Find row r such that r(r+1)/2 <= n < (r+1)(r+2)/2. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    idx_t start = r * (r + 1) / 2;
    idx_t j = n - start;            /* column within the row */

    if (j == 0)
        return (seq_t)1;            /* first element of every row */

    idx_t N = 2 * r - 1;            /* n in binomial C(2n-1, 2k-1) */
    idx_t K = 2 * j - 1;            /* k in binomial C(2n-1, 2k-1) */

    return binom(N, K);
}
