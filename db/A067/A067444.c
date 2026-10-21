/*
 * A067444  Smallest n-th power starting with 3.
 * https://oeis.org/A067444
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 16, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Find the smallest integer of the form b^n that begins with digit 3.
   a(n) = smallest b^n such that the first decimal digit of b^n is 3. */
/* The algorithm searches for the smallest base b with
   fractional part of n * log10(b) in [log10(3), log10(4)). */

#include <math.h>

seq_t a(idx_t n)
{
    /* Precomputed decimal logarithms of 3 and 4 for the inclusion interval. */
    static const long double low  = log10l(3.0L); /* ≈ 0.477121254719 */
    static const long double high = log10l(4.0L); /* ≈ 0.602059991328 */

    /* Find the first base b such that leading digit of b^n is 3. */
    idx_t b = 1;
    for (;; ++b)
    {
        /* Compute fractional part of n*log10(b). */
        long double v    = (long double)n * log10l((long double)b);
        long double frac = v - floorl(v);          /* 0 <= frac < 1 */

        if (frac >= low && frac < high)
            break;   /* Found the minimal base */
    }

    /* Compute b^n using repeated squaring; seq_t is an integer type,
       so multiplication works for all supported builds (uint8, uint16, …,
       unsigned __int128, or arbitrary‐precision). */
    seq_t result = 1;
    seq_t base   = (seq_t)b;
    idx_t exp    = n;
    while (exp > 0)
    {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        if (exp)
            base *= base;
    }

    return result;
}
