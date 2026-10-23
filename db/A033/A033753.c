/*
 * A033753  Expansion of Product_{d | 41} theta_3(q^d).
 * https://oeis.org/A033753
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A033753: Expansion of Product_{d | 41} theta_3(q^d)
 *
 * Sequence is the convolution of two indicator sequences:
 *   c1(a) = 1 if a==0, 2 if a is a perfect square >0, else 0
 *   c41(b) = 1 if b==0, 2 if b is a multiple of 41 and (b/41) is a perfect
 *            square >0, else 0
 *   a(n) = Σ_{a=0}^{n} c1(a) * c41(n-a)
 *
 * The implementation iterates over all squares a <= n; for each one it
 * checks the contribution from the corresponding b = n-a.
 */

#include <stdint.h>

/* Integer square root for 64‑bit values, used by both c1 and c41 checks. */
static idx_t isqrt_u64(idx_t n) {
    idx_t lo = 0, hi = n;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        /* Use 128‑bit multiplication to avoid overflow */
        if (((unsigned __int128)mid * mid) <= n) {
            lo = mid;
        } else {
            hi = mid - 1;
        }
    }
    return lo;
}

seq_t a(idx_t n) {
    seq_t total = 0;
    idx_t max_t = isqrt_u64(n);           /* maximum t with t^2 <= n */

    for (idx_t t = 0; t <= max_t; ++t) {
        idx_t a_val = t * t;              /* a = t^2, square of t */
        seq_t coeff1 = (a_val == 0) ? 1 : 2;   /* weight from c1 */

        idx_t b_val = n - a_val;          /* b = n - a_val */

        seq_t coeff2;                     /* weight from c41 */
        if (b_val == 0) {
            coeff2 = 1;
        } else if (b_val % 41 == 0) {
            idx_t x = b_val / 41;
            idx_t r = isqrt_u64(x);
            coeff2 = (r * r == x) ? 2 : 0;
        } else {
            coeff2 = 0;
        }
        total += coeff1 * coeff2;
    }
    return total;
}
