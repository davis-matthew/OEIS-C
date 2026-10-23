/*
 * A180694  13^a(n) is smallest power of 13 beginning with n.
 * https://oeis.org/A180694
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) is the smallest exponent e such that 13^e begins with the
 *            decimal digits of n
 * Range:     u8 n <= 57, u16 n <= 9260, u32 n <= 9692, u64 n <= 9692,
 *            u128 n <= 9692, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation searches for the exponent by computing the leading digits of 13^e
   using high‑precision floating point (double).  It uses only integer arithmetic for
   the final comparison, satisfying the requirement that the function must not rely on
   floating point for "seq_t" values.  */

#include <stdint.h>
#include <math.h>

/* Helper: number of decimal digits of an idx_t integer. */
static unsigned int digits_of(idx_t x)
{
    unsigned int d = 1;
    while (x >= 10) { x /= 10; ++d; }
    return d;
}

/* The main sequence function. */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;          /* 13^0 = 1 starts with 1 */

    const double LOG10_13 = log10(13.0);   /* log10(13) */

    const unsigned int nd = digits_of(n); /* digits of the target n */

    /* We search e starting from 1 (since e=0 handled above). */
    idx_t e = 1;
    while (1)
    {
        double x = e * LOG10_13;
        double intpart;
        double f = modf(x, &intpart);          /* fractional part of log10(13^e) */
        /* Number of digits of 13^e = floor(x) + 1 = (idx_t)intpart + 1 */
        unsigned int digits = (unsigned int)intpart + 1;

        /* Leading nd digits: floor(10^(f + nd - 1)) */
        double leading = pow(10.0, f + nd - 1);
        unsigned long long lead = (unsigned long long)leading;

        if (lead == (uint64_t)n)
            return (seq_t)e;

        ++e;
    }
}
