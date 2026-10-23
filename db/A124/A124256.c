/*
 * A124256  a(n) = n^2 - A124255(n).
 * https://oeis.org/A124256
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^2 - A124255(n) * where A124255(n) is the largest
 *            integer < n^2 expressible as * a^2 + b^2 with gcd(a,b)=1, a>0,
 *            b>0
 * Range:     u8 n <= 6, u16 n <= 22, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Helper: integer square root by binary search.
 * Returns floor(sqrt(x)).  Works for any unsigned integer seq_t. */
static seq_t isqrt_seq(seq_t x)
{
    seq_t lo = 0, hi = x, ans = 0;
    while (lo <= hi) {
        seq_t mid = lo + ((hi - lo) >> 1);
        seq_t sq = mid * mid;
        if (sq == x) return mid;
        if (sq < x) {
            ans = mid;
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return ans;
}

/* Euclidean gcd for idx_t */
static idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Main sequence function.  External linkage, no static. */
seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;    /* undefined below offset, but safe */

    seq_t n2 = (seq_t)n * (seq_t)n;
    seq_t best = 0;

    for (idx_t a = 1; a < n; ++a) {
        seq_t a2 = (seq_t)a * (seq_t)a;
        seq_t rem = n2 - a2;            /* rem > 0 for a < n */
        if (rem <= 0) continue;

        seq_t b = isqrt_seq(rem);
        if (b * b == rem) b--;          /* enforce < n^2 */
        if (b == 0) continue;

        if (gcd_idx(a, (idx_t)b) == 1) {
            seq_t sum = a2 + b * b;
            if (sum > best) best = sum;
        }
    }

    return n2 - best;
}
