/*
 * A154840  Distance to nearest cube different from n.
 * https://oeis.org/A154840
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = distance to the nearest cube different from n
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer cube root: floor(cuberoot(n)) */
static inline idx_t int_cube_root(idx_t n)
{
    /* high is definitely above the real cube root for any n < 2^64-1 */
    idx_t high = (idx_t)1 << 22;   /* 4194304 > 2097151 */
    idx_t low = 0;

    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        if (mid == 0) {               /* avoid division by zero */
            low = 1;
            continue;
        }
        idx_t square = mid * mid;     /* mid <= 4194304, so square fits in idx_t */
        if (square == 0) {            /* overflow guard, though not needed here */
            low = mid + 1;
            continue;
        }
        /* compare mid^3 <= n without overflowing:
           mid^3 <= n  <=>  n / (mid*mid) >= mid
        */
        if (n / square >= mid) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return high;   /* floor cube root */
}

seq_t a(idx_t n)
{
    idx_t m = int_cube_root(n);                    /* floor cube root of n */
    seq_t lower = (seq_t)m * m * m;                /* m^3   */
    seq_t upper = (seq_t)(m + 1) * (m + 1) * (m + 1);  /* (m+1)^3 */
    seq_t nS    = (seq_t)n;

    if (lower == nS) {             /* n itself is a cube */
        if (m == 0) {               /* no smaller cube exists */
            return upper - nS;
        }
        seq_t prev = (seq_t)(m - 1) * (m - 1) * (m - 1); /* (m-1)^3 */
        seq_t d1 = nS - prev;       /* distance to previous cube */
        seq_t d2 = upper - nS;      /* distance to next cube   */
        return d1 < d2 ? d1 : d2;
    } else {                       /* n is not a cube */
        seq_t d1 = nS - lower;     /* distance to previous cube */
        seq_t d2 = upper - nS;      /* distance to next cube   */
        return d1 < d2 ? d1 : d2;
    }
}
