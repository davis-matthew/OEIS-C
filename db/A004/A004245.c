/*
 * A004245  a(n) = ceiling(10000*log(n)).
 * https://oeis.org/A004245
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceiling(10000*log(n))
 * Range:     u8 n <= 1, u16 n <= 28, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n) {
    long double v = 10000.0L * logl((long double)n);
    unsigned long long c = (unsigned long long)ceill(v);
    if (n == 1) c = 0;
    return (seq_t)c;
}
