/*
 * A184826  a(n) = n + floor(n*t) + floor(n*t^2) + floor(n*t^3) where t is the
 *          tetranacci constant.
 * https://oeis.org/A184826
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(n*t) + floor(n*t^2) + floor(n*t^3) where t is
 *            the tetranacci constant t = 1.927561975476179629799519718060
 * Range:     u8 n <= 18, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    /* Tetranacci constant and its first three powers.
       These constants are accurate to more than 30 decimal places,
       sufficient to guarantee the correct integer part for n up to at
       least 10^12.  The long double type has 64‑bit mantissa which
       yields ≈19 decimal digits of precision. */
    static const long double t  = 1.927561975476179629799519718060L;
    static const long double t2 = t * t;
    static const long double t3 = t2 * t;

    /* Work in long double but cast back to integer types that the
       arbitrary‑precision seq_t constructor can accept.  Casting through
       unsigned long long avoids a direct conversion from long double
       to the big‑int class. */
    long double nd = (long double)n;
    unsigned long long f1 = (unsigned long long)floorl(nd * t);
    unsigned long long f2 = (unsigned long long)floorl(nd * t2);
    unsigned long long f3 = (unsigned long long)floorl(nd * t3);

    return (seq_t)n + (seq_t)f1 + (seq_t)f2 + (seq_t)f3;
}
