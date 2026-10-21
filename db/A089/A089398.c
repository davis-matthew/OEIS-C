/*
 * A089398  a(n) = n-th column sum of binary digits of k*2^(k-1), where
 *          summation is over k>=1, without carrying between columns.
 * https://oeis.org/A089398
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = sum_{k=1}^n bit(k, n-k)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where bit(k, t) is the t-th (0‑based) bit of k (0 or 1) */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        idx_t shift = n - k;           /* position to inspect in k */
        if (shift < 64) {              /* avoid undefined shift */
            sum += ( (k >> shift) & 1u );
        }
        /* if shift >= 64, the bit is 0 because k < 2^64 */
    }
    return sum;
}
