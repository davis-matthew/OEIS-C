/*
 * A143359  Triangle read by rows, T(n,k) = number of symmetric ordered trees
 *          with n edges and root degree k (1 <= k <= n).
 * https://oeis.org/A143359
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r is the row such that r*(r+1)/2 ≥ n and k =
 *            n - r*(r-1)/2 (1 ≤ k ≤ r). T(r,k) is given by T(r,k) =
 *            R_{k/2}((r-k)/2) if k even Σ_{s=0}^{⌊(r-k)/2⌋} R_{(k-1)/2}(s) *
 *            binom(r-k-2s, ⌊(r-k-2s)/2⌋) if k odd with R_m(s) = (m/(2s+m)) *
 *            C(2s+m,s) (unless m=0, then R_0(0)=1, else R_0(s)=0)
 * Range:     u8 n <= 36, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binom.h"

/* Helper: binomial central coefficient G_t(t) = binom(t, floor(t/2)). */
static inline seq_t G_t(idx_t t)
{
    return binom(t, t >> 1);
}

/* Helper: R_m(s) = (m/(2s+m)) * binom(2s+m,s).  Handles m==0 specially. */
static inline seq_t R_m_s(idx_t m, idx_t s)
{
    if (m == 0u) {
        return (s == 0u) ? (seq_t)1 : (seq_t)0;
    }
    seq_t B = binom(2u * s + m, s);
    seq_t tmp = B * (seq_t)m;
    seq_t den = 2u * s + m;
    return tmp / den;
}

/* Compute T(r,k) for even k. */
static inline seq_t T_even(idx_t r, idx_t k)
{
    if ((r - k) & 1u) return (seq_t)0;          /* parity check */
    idx_t S = (r - k) >> 1u;
    idx_t m = k >> 1u;
    return R_m_s(m, S);
}

/* Compute T(r,k) for odd k. */
static inline seq_t T_odd(idx_t r, idx_t k)
{
    idx_t m = (k - 1u) >> 1u;
    idx_t tmax = (r - k) >> 1u;
    seq_t sum = (seq_t)0;
    for (idx_t s = 0u; s <= tmax; ++s) {
        seq_t r_val = R_m_s(m, s);
        idx_t t = r - k - 2u * s;
        seq_t g_val = G_t(t);
        sum += r_val * g_val;
    }
    return sum;
}

/* External function requested in the specification. */
seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)0;          /* sequence starts at n=1 */

    /* Find smallest r such that r*(r+1)/2 >= n. */
    idx_t r = 1u;
    seq_t run_sum = 1u;                     /* r(r+1)/2 for current r */
    while (run_sum < (seq_t)n) {
        ++r;
        run_sum += r;
    }

    idx_t prev = r * (r - 1u) / 2u;          /* (r-1)r/2 */
    idx_t k = n - prev;                     /* 1 ≤ k ≤ r */

    if (k == 0u) k = r;                     /* safety, should not happen */

    if (k & 1u)               /* k odd */
        return T_odd(r, k);
    else                      /* k even */
        return T_even(r, k);
}
