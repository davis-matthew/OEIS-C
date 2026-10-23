/*
 * A121857  a(n) = ceiling(e*sqrt(n)).
 * https://oeis.org/A121857
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(e * sqrt(n))
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h> /* for sqrtl, expl */

/* seq_t and idx_t are provided by the build environment. */
seq_t a(idx_t n) {
    /* For n == 0 the expression is 0 by definition. */
    if (n == 0) {
        return 0;
    }

    /* Use long double for high precision arithmetic.
     * A long double has at least 64 bits of mantissa on all
     * mainstream platforms, which is more than enough to
     * distinguish between consecutive integer values of e*sqrt(n)
     * for n <= 10^4 and far beyond in normal usage. */
    long double x = (long double)n;
    long double val = expl(1L) * sqrtl(x);

    /* ceil() applied to a long double gives a long double,
     * cast back to seq_t (which is an unsigned integer type). */
    return (seq_t)((long long)(val + 1.0L - 1e-12L));
}
