/*
 * A183870  n+floor(sqrt(5n-5)); complement of A183871.
 * https://oeis.org/A183870
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor(sqrt(5n-5))
 * Range:     u8 n <= 52, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square‑root for an unsigned integer type seq_t.
   Returns the greatest integer r such that r*r <= x. */
static seq_t
isqrt(seq_t x)
{
    seq_t low = 0;
    seq_t high = x;
    seq_t ans = 0;
    while (low <= high) {
        seq_t mid = (low + high) >> 1;          /* mid = floor((low+high)/2) */
        if (mid == 0) {
            ans = 0;
            low = 1;
            continue;
        }
        /* Compare mid with x/mid to avoid overflow. */
        if (mid <= x / mid) {
            ans = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return ans;
}

seq_t
a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset 0, return 0 */

    seq_t val = (seq_t)n;                 /* n as seq_t */
    seq_t inside = (seq_t)5 * val - 5;    /* 5n-5 */

    seq_t sq = isqrt(inside);             /* floor(sqrt(5n-5)) */
    return val + sq;
}
