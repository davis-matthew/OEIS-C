/*
 * A122366  Triangle read by rows: T(n,k) = binomial(2*n+1,k), 0 <= k <= n.
 * https://oeis.org/A122366
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = binomial(2*m+1, k) * where m = floor((sqrt(8*n+1)-1)/2),
 *            k = n - m*(m+1)/2. * This is the triangle read by rows
 *            T(m,k)=C(2*m+1,k)
 * Range:     u8 n <= 18, u16 n <= 52, u32 n <= 169, u64 n <= 623,
 *            u128 n <= 2270, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Integer square root of an unsigned 64‑bit integer.
 * Uses binary search on idx_t (uint64_t or larger). */
static inline idx_t isqrt_u64(idx_t x)
{
    idx_t lo = 0, hi = x, ans = 0;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= (unsigned __int128)x) {
            ans = mid;
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return ans;
}

/* Small integer GCD for idx_t values (used only for cancellation
 * in the binomial calculation). */
static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Compute binomial(n,k) exactly in seq_t, using multiplicative
 * cancellation to avoid overflow of intermediate values.
 * n and k are small (n <= 300, k <= 150 for this sequence). */
static inline seq_t binomial_small(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k == 0 || k == n) return 1;
    if (k > n - k) k = n - k;   /* Use symmetry */

    /* Arrays of numerators and denominators.  k <= 150 so fixed size is safe. */
    idx_t nums[200];
    idx_t dens[200];

    for (idx_t i = 0; i < k; ++i) {
        nums[i] = n - k + 1 + i;   /* n - k + 1, ..., n */
        dens[i] = i + 1;           /* 1, 2, …, k */
    }

    /* Cancel common factors from numerators and denominators. */
    for (idx_t i = 0; i < k; ++i) {
        idx_t d = dens[i];
        if (d == 1) continue;
        for (idx_t j = 0; j < k && d > 1; ++j) {
            idx_t g = gcd_idx(nums[j], d);
            if (g > 1) {
                nums[j] /= g;
                d /= g;
            }
        }
        /* Any remaining factor in d is 1 (should not happen). */
    }

    /* Multiply the remaining numerators into the result. */
    seq_t res = 1;
    for (idx_t i = 0; i < k; ++i) {
        res *= (seq_t)nums[i];
    }
    return res;
}

/* Public sequence function required by the framework. */
seq_t a(idx_t n)
{
    /* Determine row m such that m*(m+1)/2 <= n < (m+1)*(m+2)/2. */
    idx_t t = 8 * n + 1;
    idx_t sqrt_t = isqrt_u64(t);          /* floor(sqrt(8*n+1)) */
    idx_t m = (sqrt_t - 1) / 2;           /* row number */
    idx_t offset = m * (m + 1) / 2;       /* first index of this row */
    idx_t k = n - offset;                 /* column in the row */

    /* Return binomial(2*m + 1, k). */
    return binomial_small(2 * m + 1, k);
}
