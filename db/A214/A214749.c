/*
 * A214749  Least m > 0 such that n - m divides n^2 + m.
 * https://oeis.org/A214749
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = least m > 0 such that (n - m) | (n^2 + m). Equivalent to:
 *            let d = n - m (1 ≤ d < n). Then d | n^2 + n. The largest divisor
 *            d of n^2 + n that is < n gives the minimal m as m = n - d. This
 *            implementation searches d from n-1 down to 1
 * Range:     u8 n <= 16, u16 n <= 297, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n) {
    /* For indices below the offset (n < 2) the value is undefined.
       Here we simply return 0, but the function is intended to be
       called with n >= 2. */
    if (n < 2) return 0;

    /* Compute n^2 + n as a seq_t to avoid overflow. */
    seq_t N = (seq_t)n * ((((seq_t)n) + 1));

    /* Search for the largest divisor d of N that is < n. */
    for (idx_t d = n - 1; d > 0; --d) {
        if (N % (seq_t)d == 0) {
            return (seq_t)(n - d);   /* m = n - d */
        }
    }

    /* This point is unreachable because d = 1 always divides N. */
    return 0;
}
