/*
 * A133935  A007318 * A093178 as a diagonalized matrix.
 * https://oeis.org/A133935
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = binom(r, k) * d(k) where r is the largest integer with
 *            r(r+1)/2 ≤ n, k = n - r(r+1)/2, d(k) = 1 if k is odd, otherwise
 *            k+1
 * Range:     u8 n <= 39, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline seq_t binom_idx(idx_t n_, idx_t k_)
{
    seq_t bin = 1;
    if (k_ > n_ - k_)
        k_ = n_ - k_;
    for (idx_t i = 1; i <= k_; ++i)
    {
        bin = bin * (n_ - k_ + i);
        bin = bin / i;
    }
    return bin;
}

seq_t a(idx_t n)
{
    /* Find r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2 */
    idx_t low = 0;
    idx_t high = n + 1;               /* exclusive upper bound   */
    while (low < high)
    {
        idx_t mid = (low + high) >> 1;
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri > n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low - 1;

    /* Column index within the selected row */
    idx_t start = (unsigned __int128)r * (r + 1) / 2;
    idx_t k = n - start;               /* 0 ≤ k ≤ r               */

    /* Diagonal value d(k) */
    seq_t d = (k & 1ULL) ? 1 : (seq_t)(k + 1ULL);

    /* Binomial coefficient binom(r, k) */
    seq_t b = binom_idx(r, k);

    /* Result = binom(r,k) * d(k) */
    return b * d;
}
