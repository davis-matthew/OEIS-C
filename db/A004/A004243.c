/*
 * A004243  a(n) = floor(10000*log(n)).
 * https://oeis.org/A004243
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(10000*log(n))
 * Range:     u8 n <= 1, u16 n <= 28, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

seq_t a(idx_t n) {
    long double v = 10000.0L * logl((long double)n);
    unsigned long long k = (unsigned long long)floorl(v);
    /* guard against rounding right at an integer boundary */
    long double kk = (long double)(k + 1);
    if (expl(kk / 10000.0L) <= (long double)n) k++;
    else if (k > 0 && expl((long double)k / 10000.0L) > (long double)n) k--;
    return (seq_t)k;
}
