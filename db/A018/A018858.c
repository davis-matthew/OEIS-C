/*
 * A018858  3^a(n) is smallest power of 3 beginning with n.
 * https://oeis.org/A018858
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 73, u16 n <= 8837, u32 n <= 9828, u64 n <= 9828,
 *            u128 n <= 9828, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A018858 – a(n) = smallest exponent e ≥ 0 such that 3^e starts with the decimal digits of n */

#include <stdint.h>
#include <math.h>

/* external linkage required by the build system */
seq_t a(idx_t n)
{
    /* the sequence is defined only for n ≥ 1 – for n == 0 we return 0 */
    if (n == 0) return (seq_t)0;

    /* count the number of decimal digits of n */
    int digits = 0;
    idx_t tmp = n;
    do {
        ++digits;
        tmp /= 10U;
    } while (tmp);

    /* logarithm of 3, computed once */
    static long double log3 = 0.0L;
    if (log3 == 0.0L) {
        log3 = log10l((long double)3.0L);
    }

    /* initial exponent large enough to guarantee at least 'digits' digits */
    idx_t e = (idx_t)ceill(((long double)(digits - 1)) / log3);

    /* fractional part of e * log10(3) */
    long double frac = fmodl((long double)e * log3, 1.0L);

    for (;;) {
        /* leading digits of 3^e are obtained from 10^(frac + digits - 1) */
        long double val = powl((long double)10.0L, frac + (long double)(digits - 1));
        uint64_t lead = (uint64_t)(val + 1e-12L);   /* truncate upward error */

        if (lead == n) {
            return (seq_t)e;   /* conversion to seq_t is safe because e fits */
        }

        /* advance to next exponent */
        frac += log3;
        if (frac >= 1.0L) frac -= 1.0L;
        ++e;
    }
}
