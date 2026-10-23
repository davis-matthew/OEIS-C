/*
 * A180005  Numbers n with the property that the relation 1-d!/((d-i)!d^i)>1/2
 *          holds for integers d>2 between i-1 and n+i-1.
 * https://oeis.org/A180005
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A180005: for each i ≥ 1 find the largest n such that
   1 – d! / ((d‑i)! d^i) > 1/2 holds for all integers d with i ≤ d < i+n.
   The function returns the value a(i) defined by the OEIS entry. */
#include <math.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    if (n <= 2) return (seq_t)0;          /* a(1) = a(2) = 0 */
    /* Working variable as long long to avoid arithmetic with huge idx_t ranges */
    idx_t i = n;                          /* i is the current index (starting at 3) */
    idx_t lo = i;
    /* Upper bound: roughly i^2 / (2 ln 2) ≈ 1.4427 i^2, use a safe overestimate */
    idx_t hi = i * i + 10 * i + 10;       /* safe enough for i ≤ 10000 */

    /* Binary search for the first d where the inequality fails: f(d) >= 0 */
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        long double d_ld = (long double)mid;
        long double i_ld = (long double)i;
        /* Compute f(d) = ln 2 + ln(d!) - ln((d-i)!) - i * ln d */
        long double f = logl(2.0L)
                        + lgamma(d_ld + 1.0L)
                        - lgamma(d_ld - i_ld + 1.0L)
                        - i_ld * logl(d_ld);
        /* Use a small tolerance to avoid boundary misclassifications */
        if (f < -1e-14L) {
            /* Inequality holds for mid, search higher d */
            lo = mid + 1;
        } else {
            hi = mid;                    /* first failure at mid */
        }
    }
    /* lo is the first d where the inequality fails.  
       The maximal n is lo - i. */
    return (seq_t)(lo - i);
}
