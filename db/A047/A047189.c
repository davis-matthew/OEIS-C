/*
 * A047189  Number of nonempty subsets of {1,2,...,n} in which exactly 3/5 of
 *          the elements are <= (n-2)/2.
 * https://oeis.org/A047189
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{t=1}^{⌊n/5⌋} binom(⌊(n-2)/2⌋, 3t) *
 *            binom(n-⌊(n-2)/2⌋, 2t)
 * Range:     u8 n <= 12, u16 n <= 21, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count nonempty subsets of {1,…,n} in which exactly 3/5 of the elements are ≤(n-2)/2 */

static seq_t binom(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k == 0 || k == n) return 1;
    if (k > n - k) k = n - k;

    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
        res = (res * (n - k + i)) / i;
    return res;
}

seq_t a(idx_t n)
{
    /* floor((n-2)/2) with unsigned arithmetic */
    idx_t small = (n <= 2ULL) ? 0ULL : (n - 2ULL) / 2ULL;
    idx_t large = n - small;

    idx_t tmax = n / 5ULL;      /* largest t such that 5t ≤ n */
    seq_t result = 0;

    for (idx_t t = 1; t <= tmax; ++t)
    {
        idx_t k_small = 3 * t;
        idx_t k_large = 2 * t;

        if (k_small > small || k_large > large)
            continue;

        seq_t term_small = binom(small, k_small);
        seq_t term_large = binom(large, k_large);
        result += term_small * term_large;
    }

    return result;
}
