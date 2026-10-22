/*
 * A004229  a(n) = 10000*log_10(n) rounded to the nearest integer.
 * https://oeis.org/A004229
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round(10000*log_10(n))
 * Range:     u8 n <= 1, u16 n <= 29, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* log is taken of the index only; the result is small (< 200000 for
       64-bit n), so it is safe to go through a native integer */
    long double x = 10000.0L * log10l((long double)n);
    long long r = (long long)floorl(x + 0.5L);
    seq_t res = (seq_t)0;
    res += (seq_t)r;
    return res;
}
