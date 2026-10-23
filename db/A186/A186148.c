/*
 * A186148  Rank of (1/4)n^3 when {(1/4)i^3: i>=1} and {j^2>: j>=1} are
 *          jointly ranked with (1/4)i^3 before j^2 when (1/4)i^3=j^2.
 *          Complement of A186149.
 * https://oeis.org/A186148
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor(sqrt((1/4)*n^3 - 1/8))
 * Range:     u8 n <= 5, u16 n <= 32, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          = n + floor(sqrt((2*n^3 - 1) / 8))            */

#include <stdbool.h>

/* Integer square‑root for any unsigned seq_t: floor(sqrt(x)). */
static seq_t
isqrt_seq(seq_t x)
{
    seq_t low  = 0;
    seq_t high = x;
    seq_t ans  = 0;

    while (low <= high) {
        seq_t mid = (low + high) >> 1;
        if (mid == 0) {            /* avoid division by zero */
            ans = 0;
            low = 1;               /* continue to search for larger values */
            continue;
        }
        /* compare mid*mid with x without overflow: mid <= x/mid iff mid^2 <= x */
        if (mid > x / mid) {       /* mid^2 > x */
            if (mid == 0) break;
            high = mid - 1;
        } else {                   /* mid^2 <= x */
            ans = mid;
            low = mid + 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* OEIS offset starts at 1 */

    seq_t nn = (seq_t)n;
    seq_t t  = 2 * nn * nn * nn - (seq_t)1;   /* 2*n^3 - 1 */
    seq_t k  = isqrt_seq(t / (seq_t)8);        /* floor( sqrt( (2*n^3-1)/8 ) ) */
    return nn + k;
}
