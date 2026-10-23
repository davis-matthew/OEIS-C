/*
 * A047197  Number of nonempty subsets of {1,2,...,n} in which exactly 3/4 of
 *          the elements are <= n/3.
 * https://oeis.org/A047197
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{m≥1, 3m ≤ ⌊n/3⌋, m ≤ n-⌊n/3⌋} C(⌊n/3⌋, 3m) ·
 *            C(n-⌊n/3⌋, m)
 * Range:     u8 n <= 17, u16 n <= 31, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t binom(idx_t n, idx_t k) {
    if (k > n) return 0;
    if (k == 0 || k == n) return 1;
    if (k > n - k) k = n - k;              /* use symmetry */
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (n - k + i);           /* multiply by next numerator factor */
        res = res / i;                     /* exact division by i */
    }
    return res;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;                  /* for safety; OEIS offset is 1 */
    idx_t l = n / 3;                       /* floor(n/3) */
    idx_t u = n - l;                       /* upper part size */
    idx_t M = l / 3;                       /* maximum m from 3m ≤ l */
    if (u < M) M = u;                      /* also require m ≤ u */

    seq_t ans = 0;
    for (idx_t m = 1; m <= M; ++m) {
        idx_t three_m = 3 * m;
        seq_t lower = binom(l, three_m);
        seq_t upper = binom(u, m);
        ans += lower * upper;
    }
    return ans;
}
