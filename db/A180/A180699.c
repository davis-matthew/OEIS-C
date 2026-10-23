/*
 * A180699  Smallest power of 16 that begins with n.
 * https://oeis.org/A180699
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest power of 16 that begins with the decimal digits
 *            of n
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 2, u64 n <= 2, u128 n <= 8,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>
#include "../utils/big_pow.h"

static const long double LOG16L = log10l((long double)16.0L);
static const long double EPS    = 1e-18L;

/* Count the decimal digits of a 64‑bit unsigned integer */
static int digit_count_uint64(uint64_t x)
{
    int d = 0;
    do { ++d; x /= 10; } while (x);
    return d;
}

seq_t a(idx_t n)
{
    uint64_t nu    = (uint64_t)n;
    int      digits = digit_count_uint64(nu);

    /* fractional part of log10(n) and log10(n+1) */
    long double logn  = log10l((long double)nu);
    long double low   = logn  - floorl(logn);
    long double logn1 = log10l((long double)(nu + 1));
    long double high  = logn1 - floorl(logn1);

    /* smallest k such that 16^k has at least `digits` digits */
    long double req = ((long double)(digits - 1)) / LOG16L;
    idx_t k = (idx_t)ceill(req);

    /* fractional part of k*log10(16) */
    long double f = (long double)k * LOG16L;
    f -= floorl(f);

    while (1) {
        if (high <= low) {
            if (f + EPS >= low) {
                return big_pow((seq_t)16, (seq_t)k);
            }
        } else {
            if ((f + EPS >= low) && (f < high - EPS)) {
                return big_pow((seq_t)16, (seq_t)k);
            }
        }

        /* advance to the next exponent */
        ++k;
        f += LOG16L;
        if (f >= 1.0L) f -= floorl(f);
    }
}
