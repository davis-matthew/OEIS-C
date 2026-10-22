/*
 * A001195  a(n) = floor(n*log((14/11)*n^(10/9))).
 * https://oeis.org/A001195
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*log((14/11)*n^(10/9))) = floor(n*(log(14/11) +
 *            (10/9)*log(n)))
 * Range:     u8 n <= 54, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

seq_t a(idx_t n) {
    long double dn = (long double)n;
    long double val = dn * (logl(14.0L / 11.0L) + (10.0L / 9.0L) * logl(dn));
    long double f = floorl(val);
    unsigned long long r = (f > 0.0L) ? (unsigned long long)f : 0ULL;
    return (seq_t)r;
}
