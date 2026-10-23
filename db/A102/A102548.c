/*
 * A102548  Number of positive integers <= n that are expressible in the form
 *          u^2+v^2, with u and v integers.
 * https://oeis.org/A102548
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = #{ m | 1 ≤ m ≤ n and m = u² + v² }
 * Range:     u8 n <= 756, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Integer square root for idx_t (assumed unsigned, e.g., uint64_t). */
static inline idx_t int_sqrt(idx_t x)
{
    idx_t lo = 0, hi = (x >> 1) + 1, mid, res = 0;
    while (lo <= hi) {
        mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == x) return mid;
        if (sq < x) {
            res = mid;
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* Allocate flag array */
    uint8_t *seen = (uint8_t *)malloc((size_t)(n + 1));
    if (!seen) return 0;
    for (idx_t i = 0; i <= n; ++i) seen[i] = 0;

    idx_t amax = int_sqrt(n);
    for (idx_t a = 0; a <= amax; ++a) {
        idx_t aa = a * a;
        idx_t remain = n - aa;
        idx_t bmax = int_sqrt(remain);
        for (idx_t b = 0; b <= bmax; ++b) {
            idx_t bb = b * b;
            idx_t s = aa + bb;   /* s ≤ n by construction */
            seen[s] = 1;
        }
    }

    seq_t count = 0;
    for (idx_t i = 1; i <= n; ++i) {
        if (seen[i]) ++count;
    }

    free(seen);
    return count;
}
