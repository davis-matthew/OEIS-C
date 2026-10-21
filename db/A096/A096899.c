/*
 * A096899  Least inradius of primitive Pythagorean triangles with even leg
 *          4n.
 * https://oeis.org/A096899
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2n - d_max², where d_max is the largest divisor of 2n
 *            that is less than sqrt(2n) and coprime with 2n/d_max
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

static inline idx_t gcd_idx(idx_t a, idx_t b) {
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    idx_t N = 2 * n;
    /* largest integer d with d*d < N */
    idx_t limit = (idx_t)floor(sqrt((long double)N));
    idx_t d;
    for (d = limit; d >= 1; --d) {
        if (N % d == 0) {
            idx_t other = N / d;          /* the co‑factor m */
            if (gcd_idx(d, other) == 1) {
                break;
            }
        }
    }
    /* d is at least 1, so this write is valid */
    seq_t res = (seq_t)(N - d * d);
    return res;
}
