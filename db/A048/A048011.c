/*
 * A048011  Number of nonempty subsets of {1,2,...,n} in which exactly 2/5 of
 *          the elements are <= (n-1)/3.
 * https://oeis.org/A048011
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{j>=1} binom( floor((n-1)/3), 2*j ) * binom( n -
 *            floor((n-1)/3), 3*j )
 * Range:     u8 n <= 12, u16 n <= 20, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t binom_fast(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i) / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    idx_t m = (n - 1) / 3;
    idx_t n_m = n - m;
    idx_t j_max = m / 2;
    if ((n_m / 3) < j_max) j_max = n_m / 3;
    seq_t total = 0;
    for (idx_t j = 1; j <= j_max; ++j) {
        idx_t k1 = 2 * j;
        idx_t k2 = 3 * j;
        seq_t c1 = binom_fast(m, k1);
        seq_t c2 = binom_fast(n_m, k2);
        total += c1 * c2;
    }
    return total;
}
