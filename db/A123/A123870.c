/*
 * A123870  Order of minimal triangulation of nonorientable closed surface
 *          with n cross-caps (N_n).
 * https://oeis.org/A123870
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil((7 + sqrt(1+24*n))/2), except a(2) = 8, a(3) = 9
 * Range:     u8 n <= 3, u16 n <= 36, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 90 terms through n = 998 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Integer square root (floor) of a non‑negative seq_t value. */
static seq_t isqrt_seq(seq_t v)
{
    seq_t lo = 0, hi = v;
    while (lo <= hi) {
        seq_t mid = lo + ((hi - lo) >> 1);
        seq_t sq  = mid * mid;
        if (sq == v) return mid;
        if (sq < v) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return hi;
}

seq_t a(idx_t n) {
    /* Handle the special cases first */
    if (n == 2) return 8;
    if (n == 3) return 9;

    seq_t nn = n;                 /* convert index to seq_t */
    seq_t disc = 1 + 24 * nn;     /* discriminant 1 + 24*n */
    seq_t t = isqrt_seq(disc);    /* floor(sqrt(disc)) */

    /* Check if disc is a perfect square */
    if (t * t == disc) {
        return (7 + t) / 2;       /* exact value when sqrt is integer */
    } else {
        return (7 + t) / 2 + 1;   /* ceil when sqrt is not integer */
    }
}
