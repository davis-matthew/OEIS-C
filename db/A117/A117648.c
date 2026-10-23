/*
 * A117648  Let f(0) = 2, f(1) = 3, and f(n) = (4/3)*f(n-1) - f(n-2) for n >=
 *          2, a(n) = abs(floor(f(n))).
 * https://oeis.org/A117648
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = abs(floor(f(n))), f(0)=2, f(1)=3, f(n) = (4/3)*f(n-1) -
 *            f(n-2) (n >= 2)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The value is computed using double arithmetic with careful handling
   of floor for both positive and negative results. */

#include <stdint.h>
#include <math.h>

seq_t a(idx_t n) {
    /* Base cases */
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)3;

    /* Compute f(n) iteratively */
    double f_prev = 2.0;   /* f(0) */
    double f_curr = 3.0;   /* f(1) */
    double f_next = 0.0;
    for (idx_t i = 2; i <= n; ++i) {
        f_next = (4.0 / 3.0) * f_curr - f_prev;
        f_prev = f_curr;
        f_curr = f_next;
    }

    /* Determine floor(f(n)) accurately */
    double val = f_curr;
    long long nearest = llround(val);          /* nearest integer */
    long long floor_val;
    const double eps = 1e-9;                  /* safety epsilon */
    if (fabs(val - nearest) < eps) {
        floor_val = nearest;                  /* close to integer */
    } else if (val >= 0.0) {
        floor_val = (long long)val;           /* truncates toward zero,
                                                works for positive */
    } else {
        floor_val = (long long)val - 1;       /* adjust for negative */
    }

    /* Return absolute value as seq_t */
    long long abs_val = floor_val >= 0 ? floor_val : -floor_val;
    return (seq_t)abs_val;
}
