/*
 * A004268  a(n) = floor(10000*log_2(n)).
 * https://oeis.org/A004268
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(10000*log_2(n)); exact when n is a power of 2,
 *            otherwise computed in long double
 * Range:     u8 n <= 1, u16 n <= 93, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

seq_t a(idx_t n) {
    long long e = 0, k;
    idx_t m = n;
    long double r;
    if (n <= 1) return (seq_t)0;
    while (m > 1) { m >>= 1; e++; }
    if ((n & (n - 1)) == 0)
        return (seq_t)10000 * (seq_t)e;
    r = 10000.0L * log2l((long double)n);
    k = (long long)floorl(r);
    return (seq_t)k;
}
