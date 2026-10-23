/*
 * A159230  1/8 the number of n X n arrays of squares of integers with every
 *          2X2 subblock summing to 31.
 * https://oeis.org/A159230
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = # of n×n arrays of integer squares with every 2×2
 *            subblock summing to 31, divided by 8. Computed by the recurrence
 *            a(2)=2, a(3)=5, for n≥4: let d(3)=3, for even n:
 *            d(n)=2·d(n−1)−2^{n/2−2}, for odd n: d(n)=2·d(n−1), and
 *            a(n)=a(n−1)+d(n)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n <= 2) return (seq_t)2;

    seq_t a_val = (seq_t)2;
    seq_t diff   = (seq_t)0;      /* d(i−1) will be stored in diff when loop reaches i */
    seq_t pow_even = (seq_t)1;    /* 2^{i/2−2} for the current even i, starts with 1 for i=4 */

    for (idx_t i = 3; i <= n; ++i) {
        if (i == 3) {
            diff = (seq_t)3;            /* d(3) */
        } else if ((i & 1ULL) == 0) {   /* even i ≥ 4 */
            diff = (diff << 1) - pow_even;
            pow_even <<= 1;             /* prepare 2^{i/2−2} for next even i */
        } else {                        /* odd i ≥ 5 */
            diff = diff << 1;           /* d(i) = 2·d(i−1) */
        }
        a_val += diff;                  /* a(i) = a(i−1) + d(i) */
    }
    return a_val;
}
