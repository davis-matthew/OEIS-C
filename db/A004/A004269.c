/*
 * A004269  a(n) = round(10000*log_2(n)).
 * https://oeis.org/A004269
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round(10000*log_2(n))
 * Range:     u8 n <= 1, u16 n <= 34, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n) {
    long double v = 10000.0L * log2l((long double)n);
    long long r = (long long)floorl(v + 0.5L);
    return (seq_t)r;
}
