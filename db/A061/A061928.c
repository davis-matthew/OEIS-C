/*
 * A061928  Array T(n,m) = 1/beta(n+1,m+1) read by antidiagonals.
 * https://oeis.org/A061928
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy, nice
 * Formula:   a(n) = (k+1)!/(n_i!·m_i!) where n_i+m_i=k, k = smallest integer
 *            ≥2 satisfying (k-1)·k/2 ≥ n and n_i = n - (k-2)·(k-1)/2. This is
 *            equivalent to a(n) = binomial(k+1, n_i) * (k - n_i + 1). The
 *            terms are read by antidiagonals of the array T(n,m)=1/β(n+1,m+1)
 * Range:     u8 n <= 17, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

/* Integer square root for 64‑bit unsigned values. */
static idx_t floor_sqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = ((idx_t)1 << 32);   /* sqrt(2^64−1) < 2^32 */
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo + 1) >> 1);
        if ((unsigned __int128)mid * mid <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;           /* outside the OEIS offset */

    /* Find the diagonal k: smallest k ≥ 2 with (k‑1)·k/2 ≥ n. */
    idx_t disc = 1 + 8 * n;
    idx_t s    = floor_sqrt_u64(disc);
    idx_t k    = (1 + s) >> 1;            /* (1 + s)/2 */
    if ((k - 1) * k / 2 < n) ++k;         /* ensure the inequality */

    /* Number of terms preceding this diagonal. */
    idx_t prev = (k - 2) * (k - 1) / 2;

    /* Index within the diagonal (0‑based). */
    idx_t i    = n - prev - 1;            /* 0 … k‑2 */
    idx_t n_i  = i + 1;                   /* 1 … k‑1 */

    /* Compute the binomial coefficient and multiply by (k‑n_i+1). */
    seq_t binom = binomial(k + 1, n_i);
    seq_t factor = (seq_t)(k - n_i + 1);
    return binom * factor;
}
