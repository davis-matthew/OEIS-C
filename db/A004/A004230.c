/*
 * A004230  a(n) = 10000*log_10(n) rounded up.
 * https://oeis.org/A004230
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceiling(10000*log_10(n)); exact powers of 10 handled
 *            separately
 * Range:     u8 n <= 1, u16 n <= 29, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* exact powers of 10 give an integer result directly */
    idx_t m = n;
    long long e = 0;
    while (m >= 10 && m % 10 == 0) {
        m /= 10;
        e++;
    }
    if (m == 1)
        return (seq_t)(e * 10000LL);

    long double t = 10000.0L * log10l((long double)n);
    long long k = (long long)ceill(t);
    return (seq_t)k;
}
