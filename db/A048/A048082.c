/*
 * A048082  Number of nonempty subsets of {1,2,...,n} in which exactly 1/2 of
 *          the elements are <= (n+3)/3.
 * https://oeis.org/A048082
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = Σ_{m=1}^{min(t, n−t)} C(t, m) * C(n−t, m) * where t =
 *            ⌊(n+3)/3⌋. * This counts nonempty subsets of {1,…,n} with
 *            exactly one half of * the elements <= t
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t binom(idx_t n, idx_t k)
{
    /* compute binomial coefficient C(n, k) iteratively, avoiding overflow */
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        seq_t numerator   = (seq_t)(n - k + i);
        seq_t denominator = (seq_t)i;
        /* division is exact */
        res = (res * numerator) / denominator;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset not used, but safe */
    idx_t t = (n + 3) / 3;         /* floor((n+3)/3) */
    idx_t small = t;
    idx_t large = n - t;
    idx_t mmax = small < large ? small : large;

    seq_t sum = 0;
    for (idx_t m = 1; m <= mmax; ++m) {
        seq_t c1 = binom(small, m);
        seq_t c2 = binom(large, m);
        sum += c1 * c2;
    }
    return sum;
}
