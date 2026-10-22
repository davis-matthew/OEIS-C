/*
 * A004237  a(n) = floor(100*log(n)).
 * https://oeis.org/A004237
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(100*log(n)), with log computed in long double on
 *            the index (the result is at most about 4437, so it fits in a
 *            native integer)
 * Range:     u8 n <= 12, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n) {
    long double r = 100.0L * logl((long double)n);
    long long k = (long long)floorl(r);
    if (k < 0) k = 0;
    return (seq_t)k;
}
