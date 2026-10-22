/*
 * A004228  a(n) = 10000*log_10(n) rounded down.
 * https://oeis.org/A004228
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(10000*log_10(n)); exact when n is a power of 10,
 *            otherwise computed in long double on the index
 * Range:     u8 n <= 1, u16 n <= 29, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

seq_t a(idx_t n) {
    idx_t m = n;
    long long d = 0;
    while (m % 10 == 0) {
        m /= 10;
        d++;
    }
    if (m == 1)
        return (seq_t)(10000LL * d);
    long double v = 10000.0L * log10l((long double)n);
    long long k = (long long)floorl(v);
    return (seq_t)k;
}
