/*
 * A004002  Benford numbers: a(n) = e^e^...^e (n times) rounded to nearest
 *          integer.
 * https://oeis.org/A004002
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round(e^e^...^e) with n copies of e, i.e. x(0) = 1, x(n)
 *            = exp(x(n-1)), a(n) = round(x(n))
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

seq_t a(idx_t n) {
    long double x = 1.0L; /* x(0) = e^0 = 1 */
    idx_t i;
    uint64_t r;
    if (n > 3)
        return (seq_t)0; /* a(4) already has 1656521 digits */
    for (i = 0; i < n; i++)
        x = expl(x);
    r = (uint64_t)(x + 0.5L);
    return (seq_t)r;
}
