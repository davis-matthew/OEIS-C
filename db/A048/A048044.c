/*
 * A048044  Number of nonempty subsets of {1,2,...,n} in which exactly 2/5 of
 *          the elements are <= (n+1)/3.
 * https://oeis.org/A048044
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{t≥1} C(⌊(n+1)/3⌋,2t) * C(n-⌊(n+1)/3⌋,3t) (t ranges
 *            over integers such that 2t≤⌊(n+1)/3⌋ and 3t≤n-⌊(n+1)/3⌋)
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Multiply‑divide binomial coefficient C(n,k).  Designed for any seq_t
   type that supports + - * / and truncating integer division. */
static seq_t binom_simple(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;          /* use symmetry */
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i) / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* m = floor((n+1)/3) – number of elements ≤ (n+1)/3 */
    idx_t m = (n + 1) / 3;
    idx_t n_m = n - m;                 /* elements > (n+1)/3 */

    seq_t total = (seq_t)0;
    /* t must satisfy 2t ≤ m and 3t ≤ n_m; t≥1 ensures nonempty subsets */
    idx_t t_max = n / 5;
    if (2 * t_max > m) t_max = m / 2;  /* enforce 2t ≤ m */
    if (3 * t_max > n_m) t_max = n_m / 3;   /* enforce 3t ≤ n_m */

    for (idx_t t = 1; t <= t_max; ++t) {
        idx_t a = 2 * t;   /* elements from first group */
        idx_t b = 3 * t;   /* elements from second group */
        seq_t cnt1 = binom_simple(m, a);
        seq_t cnt2 = binom_simple(n_m, b);
        total += cnt1 * cnt2;
    }

    return total;
}
