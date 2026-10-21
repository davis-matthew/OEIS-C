/*
 * A212101  Number of (w,x,y,z) with all terms in {1,...,n} and w*x^2=y*z^2.
 * https://oeis.org/A212101
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = Σ_{1≤u,v≤⌊√n⌋, gcd(u,v)=1} ⌊min(n/u, n/v)⌋ * ⌊min(n/u², n/v²)⌋
 * (sum over coprime pairs (u,v) of the maximum allowed scalars for the
 *   parameters in the parametrization of the solutions)
 */
#include <stdint.h>
#include <stdbool.h>

/* Compute integer gcd for idx_t (unsigned 64‑bit). */
static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Integer floor of square root using no overflow. */
static inline idx_t floor_sqrt(idx_t n)
{
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        if (mid > 0 && mid > n / mid) {
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    return high;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    const idx_t sq = floor_sqrt(n);                 /* ⌊√n⌋ */
    seq_t total = 0;

    for (idx_t u = 1; u <= sq; ++u) {
        const idx_t u2 = u * u;                      /* u² (safe, u ≤ √n) */
        const idx_t u_limit = n / u;                 /* ⌊n/u⌋          */
        const idx_t w_limit_u = n / u2;              /* ⌊n/u²⌋          */

        for (idx_t v = 1; v <= sq; ++v) {
            if (gcd_idx(u, v) != 1) continue;

            const idx_t v2 = v * v;
            const idx_t v_limit = n / v;             /* ⌊n/v⌋          */
            const idx_t w_limit_v = n / v2;          /* ⌊n/v²⌋          */

            const idx_t dmax = u_limit < v_limit ? u_limit : v_limit;
            if (dmax == 0) continue;

            const idx_t wmax = w_limit_u < w_limit_v ? w_limit_u : w_limit_v;
            if (wmax == 0) continue;

            total += (seq_t)dmax * (seq_t)wmax;
        }
    }
    return total;
}
