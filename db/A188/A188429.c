/*
 * A188429  L(n) is the minimum of the largest elements of all n-full sets, or
 *          0 if no such set exists.
 * https://oeis.org/A188429
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   not stated
 * Range:     u8 n <= 8889, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A188429: L(n) – minimal maximum element of an n‑full set
 *
 * For n >= 15 let k = ⌊(√(8n+1) - 1)/2⌋ and r = n - k(k+1)/2.
 * Then
 *   a(n) = k          if r == 0,
 *   a(n) = k + 1      if 1 <= r <= k-2,
 *   a(n) = k + 2      if k-1 <= r <= k.
 * For 1 <= n < 15 the pre‑computed values are used.
 */
#include <stddef.h>

static idx_t isqrt_idx(idx_t x)
{
    idx_t lo = 0, hi = x, res = 0;
    while (lo <= hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (mid == 0) { res = 0; lo = 1; continue; }
        if (mid <= x / mid) {
            res = mid;
            lo = mid + 1;
        } else {
            if (mid == 0) break; /* prevent infinite loop */
            hi = mid - 1;
        }
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Small values: n = 1 … 14 */
    static const seq_t small[15] = {
        0, /* dummy for index 0 */
        1, 0, 2, 0, 0, 3, 4, 0, 0, 4, 5, 5, 6, 7
    };
    if (n < 15) {
        return small[n];
    }

    /* Compute k = floor((√(8n+1)-1)/2) */
    idx_t tmp = (idx_t)8 * n + 1;
    idx_t t = isqrt_idx(tmp);          /* t = floor(√(8n+1)) */
    seq_t k = (seq_t)(t - 1) / 2;

    /* Compute r = n - k(k+1)/2 */
    seq_t T = k * (k + 1) / 2;
    seq_t r = (seq_t)n - T;

    if (r == 0) {
        return k;
    } else if (r <= k - 2) {
        return k + 1;
    } else {
        return k + 2;
    }
}
