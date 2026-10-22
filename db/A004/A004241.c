/*
 * A004241  a(n) = 1000*log(n) rounded to the nearest integer.
 * https://oeis.org/A004241
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round(1000*log(n)), computed in long double, then rounded
 *            to a small integer before converting to seq_t
 * Range:     u8 n <= 1, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

seq_t a(idx_t n) {
    long double r;
    long long k;
    if (n <= 1) return (seq_t)0;
    r = 1000.0L * logl((long double)n);
    k = (long long)floorl(r + 0.5L);
    return (seq_t)k;
}
