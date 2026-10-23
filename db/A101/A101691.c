/*
 * A101691  A modular binomial sum sequence.
 * https://oeis.org/A101691
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=0..n} mod(binomial(2n-2, k), 2)
 * Range:     u8 n <= 511, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Special case: a(0) = 1 */
    if (n == 0) return (seq_t)1;

    /* m = 2n - 2; for n >= 1 this fits in idx_t safely */
    idx_t m = 2 * n - 2;
    seq_t sum = 0;

    for (idx_t k = 0; k <= n; ++k) {
        /* If k > m, binomial( m, k ) = 0 -> even parity -> skip */
        if (k > m) continue;

        /* Binary Lucas theorem: binomial(m,k) is odd iff (k & (m-k)) == 0 */
        if ((k & (m - k)) == 0) {
            ++sum;
        }
    }

    return sum;
}
