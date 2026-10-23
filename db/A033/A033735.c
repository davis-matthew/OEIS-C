/*
 * A033735  Expansion of Product_{d | 23} theta_3(q^d).
 * https://oeis.org/A033735
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer pairs (k,l) ∈ ℤ² with k² + 23·l² = n,
 *            counted with sign multiplicities
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root: floor(√n) for unsigned 64‑bit values.  */
static idx_t isqrt_idx(idx_t n)
{
    idx_t lo = 0, hi = n, ans = 0;
    while (lo <= hi) {
        idx_t mid = lo + (hi - lo) / 2;
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= n) {
            ans = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return ans;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t max_l = isqrt_idx(n / 23);
    for (idx_t l = 0; l <= max_l; ++l) {
        unsigned __int128 t = (unsigned __int128)23 * l * l;
        if (t > n) break;
        idx_t term = (idx_t)t;
        idx_t rest = n - term;
        idx_t k = isqrt_idx(rest);
        if ((unsigned __int128)k * k != rest) continue;

        seq_t mult;
        if (l == 0 && k == 0) {
            mult = 1;
        } else if (l == 0 || k == 0) {
            mult = 2;
        } else {
            mult = 4;
        }
        sum += mult;
    }
    return sum;
}
