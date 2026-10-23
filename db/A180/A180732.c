/*
 * A180732  23^a(n) is smallest power of 23 beginning with n.
 * https://oeis.org/A180732
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 3371, u32 n <= 9873, u64 n <= 9873,
 *            u128 n <= 9873, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Smallest exponent e such that 23^e begins with the integer n */
#include <math.h>
#include <stdint.h>
#include <stdbool.h>

/* Count decimal digits of an unsigned 64‑bit value (n ≥ 1). */
static inline unsigned count_digits(uint64_t x) {
    unsigned d = 0;
    do { d++; x /= 10; } while (x);
    return d;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)1) return (seq_t)0;          /* 23^0 = 1 starts with 1 */

    const unsigned d = count_digits((uint64_t)n);   /* digits of target n */

    /* High‑precision value of log10(23). */
    static const long double LOG10_23 = log10l((long double)23.0L);

    long double frac = 0.0L;       /* fractional part of e*log10(23)   */
    unsigned intpart = 0;          /* integer part of e*log10(23)      */
    seq_t e = 0;                   /* current exponent e               */

    /* Iterate over exponents until the leading d digits match n. */
    for (;;) {
        /* Number of decimal digits of 23^e is intpart+1. */
        if (intpart + 1U >= d) {
            /* Leading d digits are floor(10^(frac + d-1)). */
            long double ld = powl(10.0L, frac + (long double)(d - 1));
            /* Tiny epsilon to guard against catastrophic rounding. */
            const long double eps = 1e-12L;
            unsigned long long leading = (unsigned long long)(ld + eps);
            if ((unsigned long long)n == leading)
                return e;
        }

        /* Advance to the next exponent. */
        frac += LOG10_23;
        if (frac >= 1.0L) {
            frac -= floorl(frac);
            intpart++;
        }
        e++;
    }
}
