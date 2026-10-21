/*
 * A240076  Number of partitions of n such that m(greatest part) < m(1), where
 *          m = multiplicity.
 * https://oeis.org/A240076
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{L=2}^{n} Σ_{m=1}^{⌊(n‑1)/(L+1)⌋} p_{L‑1}(n ‑ m·(L+1) ‑
 *            1), where p_k(t) is the number of partitions of t with parts ≤ k
 * Range:     u8 n <= 18, u16 n <= 45, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* handle small n automatically (a(0)=a(1)=a(2)=a(3)=0) */
    if (n < 4) return (seq_t)0;

    /* allocate array for partition counts, zero-initialized */
    seq_t *p = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));
    if (!p) return (seq_t)0;
    p[0] = (seq_t)1;   /* base case: one way to partition 0 */

    seq_t ans = (seq_t)0;
    /* iterate over candidate largest part L = k+1, with k = 1 .. n‑1 */
    for (idx_t k = 1; k <= n - 1; ++k)
    {
        /* add part k to the DP: p[i] += p[i‑k] for i = k .. n */
        for (size_t i = (size_t)k; i <= (size_t)n; ++i)
            p[i] += p[i - (size_t)k];

        /* now p holds counts for parts ≤ k = L‑1 */
        /* enumerate multiplicities m of the largest part L = k+1 */
        for (idx_t m = 1;; ++m)
        {
            idx_t prod = m * (k + 2);   /* m·(L+1) = m·(k+2) */
            if (prod > n - 1) break;   /* t = n - prod - 1 would be < 0 */

            idx_t t = n - prod - 1;
            ans += p[(size_t)t];
        }
    }

    free(p);
    return ans;
}
