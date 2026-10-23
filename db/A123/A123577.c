/*
 * A123577  The Kruskal-Macaulay function L_5(n).
 * https://oeis.org/A123577
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = L_5(n) – the Kruskal‑Macaulay function for k = 5 Computed
 *            from the unique decomposition of n into binomial coefficients
 *            C(m_k,k)+C(m_{k-1},k-1)+…+C(m_1,1); then a(n)=Σ C(m_i,i+1)
 * Range:     u8 n <= 44, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Small binomial: compute C(n,k) for 0 ≤ k ≤ n (k small, n moderate). */
static seq_t binom_small(idx_t n, idx_t k)
{
    if (k > n) return 0;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        /* multiply first, then divide – exact division holds each step. */
        res = res * (n - i + 1) / i;
    }
    return res;
}

/* OEIS A123577: Kruskal‑Macaulay function L_5(n). */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t rem = n;
    idx_t k   = 5;                    /* largest binomial index to use */

    while (rem > 0 && k > 0)
    {
        /* Find largest m with binom(m,k) <= rem. */
        idx_t lo = k;                 /* binom(m,k) defined only for m ≥ k */
        idx_t hi = (rem > k) ? rem : k;
        idx_t best = lo;

        while (lo <= hi)
        {
            idx_t mid = lo + (hi - lo) / 2;
            seq_t val = binom_small(mid, k);

            if ((seq_t)rem >= val)
            {
                best = mid;
                lo   = mid + 1;
            }
            else
            {
                if (mid == 0) break; /* safety; shouldn't happen */
                hi = mid - 1;
            }
        }

        seq_t bin_val   = binom_small(best, k);        /* C(best,k) */
        seq_t add_val   = binom_small(best, k + 1);    /* C(best,k+1) */

        rem -= (idx_t)bin_val;    /* safe: bin_val <= rem */
        sum += add_val;

        k--;    /* move to next lower binomial index */
    }

    return sum;
}
