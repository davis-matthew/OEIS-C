/*
 * A047174  Number of nonempty subsets of {1,2,...,n} in which exactly 1/4 of
 *          the elements are <= (n-1)/2.
 * https://oeis.org/A047174
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{t=1}^{⌊n/4⌋} C(⌊(n-1)/2⌋, t) * C(n - ⌊(n-1)/2⌋, 3t)
 * Range:     u8 n <= 12, u16 n <= 21, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t binom_idx(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        /* multiply and divide in one step; division is exact */
        res = res * (n - i + 1) / i;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* compute m = floor((n-1)/2) and high = n - m */
    idx_t m = (n - 1) / 2;
    idx_t high = n - m;

    seq_t ans = 0;
    for (idx_t t = 1; t <= n / 4; ++t) {
        if (m < t) continue;                     /* C(m, t) would be zero */
        idx_t need = 3 * t;
        if (high < need) continue;               /* C(high, 3t) would be zero */
        seq_t c1 = binom_idx(m, t);
        seq_t c2 = binom_idx(high, need);
        ans += c1 * c2;
    }
    return ans;
}
