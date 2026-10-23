/*
 * A188795  a(n) counts all integers k in [2,floor(sqrt(n))] such that the
 *          number of divisors d>1 of n-k with k|(n-d) equals A188550(n).
 * https://oeis.org/A188795
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts the integers k in [2,⌊√n⌋] for which the number of
 *            divisors d>1 of n−k satisfying k | (n−d) is maximal among all
 *            such k. That maximal value is A188550(n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Integer square‑root (floor) for unsigned 64‑bit numbers. */
static idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0, high = n, mid, ans = 0;
    while (low <= high) {
        mid = low + (high - low) / 2;
        if (mid == 0) {          /* avoid division by zero */
            ans = 0;
            low = 1;
            continue;
        }
        if (mid <= n / mid) {
            ans = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    if (n < 4) return 0;                /* behaviour below the offset is
                                           unspecified; return 0 as a
                                           sensible default. */

    idx_t m = isqrt_u64(n);              /* floor(sqrt(n))          */
    idx_t maxcnt = 0;                    /* current maximal count   */
    idx_t ways = 0;                      /* number of k achieving
                                           the maximum count        */

    for (idx_t k = 2; k <= m; ++k) {
        idx_t x = n - k;                 /* n - k                  */
        idx_t cnt = 0;                   /* count for this k       */

        /* Enumerate divisors d>1 of x. */
        for (idx_t d = 2; d * d <= x; ++d) {
            if (x % d == 0) {
                /* divisor d */
                if ((n - d) % k == 0) ++cnt;

                /* paired divisor d2 = x/d, avoid double counting if
                   d == d2. */
                idx_t d2 = x / d;
                if (d2 != d && (n - d2) % k == 0) ++cnt;
            }
        }

        /* Include the divisor x itself (if >1). */
        if (x > 1 && (n - x) % k == 0) ++cnt;

        /* Update maximum and count of achieving k's. */
        if (cnt > maxcnt) {
            maxcnt = cnt;
            ways = 1;
        } else if (cnt == maxcnt) {
            ++ways;
        }
    }

    return (seq_t)ways;
}
