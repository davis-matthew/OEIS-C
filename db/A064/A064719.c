/*
 * A064719  A Beatty sequence for 3^i + 3^-i + 1.
 * https://oeis.org/A064719
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor((1 + 2*cos(ln(3))) * n)
 * Range:     u8 n <= 134, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    static long double alpha = 0.0L;
    if (alpha == 0.0L) {
        alpha = 1.0L + 2.0L * cosl(logl(3.0L));
    }
    unsigned long long t = (unsigned long long)(alpha * (long double)n + 1e-12L);
    return (seq_t)t;
}
