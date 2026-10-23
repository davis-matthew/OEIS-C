/*
 * A144090  Triangle read by rows: T(n,k) is the number of partial bijections
 *          (or subpermutations) of an n-element set of height k
 *          (height(alpha) = |Im(alpha)|) and with exactly 1 fixed point.
 * https://oeis.org/A144090
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 32, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = T(row, k) where
 *   row = smallest r such that r*(r+1)/2 ≥ n,
 *   k   = n - r*(r-1)/2.
 *   T(r,k) = (r!/(r-k)!) * Σ_{m=0}^{k-1} (-1)^m / m! * C(r-1-m, k-1-m).
 *
 * The function uses the helper binom(n,k) from "../utils/binom.h".
 * An integer square‑root routine is implemented locally to avoid
 * external dependencies.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/binom.h"

/* integer square root of UINT64: floor(sqrt(x)) */
static idx_t floor_sqrt_u64(idx_t x)
{
    idx_t lo = 0;
    idx_t hi = (idx_t)1ULL << 32; /* √(2^64−1) < 2^32 */
    idx_t best = 0;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        __int128 sq = (__int128)mid * mid;
        if (sq <= x) {
            best = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return best;
}

seq_t a(idx_t n)
{
    /* determine the row r (1‑based) such that r*(r+1)/2 ≥ n */
    idx_t r = (idx_t)((floor_sqrt_u64(8 * n + 1) - 1) / 2);
    while ((r * (r + 1)) / 2 < n) {
        r++;
    }

    /* column k (1‑based) within this row */
    idx_t prev_tri = (r - 1) * r / 2;
    idx_t k = n - prev_tri;               /* 1 ≤ k ≤ r */

    /* compute P = r! / (r-k)! = ∏_{i=0}^{k-1} (r - i) */
    seq_t P = (seq_t)1;
    for (idx_t i = 0; i < k; ++i) {
        P *= (seq_t)(r - i);
    }

    /* compute the alternating sum */
    seq_t sum = (seq_t)0;
    seq_t fact_m = 1;                      /* m! for m = 0 */
    for (idx_t m = 0; m < k; ++m) {
        if (m > 0) {
            fact_m *= (seq_t)m;            /* fact_m = m! */
        }

        idx_t n1 = r - 1 - m;
        idx_t k1 = k - 1 - m;
        seq_t C = binom(n1, k1);

        seq_t temp = P * C;                /* temp = P * C(n-1-m, k-1-m) */
        temp /= fact_m;                    /* exact division by m! */

        if ((m & 1) == 0) {
            sum += temp;
        } else {
            sum -= temp;
        }
    }

    return sum;
}
