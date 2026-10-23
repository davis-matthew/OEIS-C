/*
 * A106043  First digit other than 9 in the fractional part of the decimal
 *          expansion of (1/1000^n)^(1/1000^n).
 * https://oeis.org/A106043
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = 10 − ⌈10^k0 · ln10 · 3n · 10^(k0−3n)⌉ * where k0 = ⌈3n −
 *            log10(3n) − log10(ln10)⌉, n ≥ 1. * For n = 0, a(0) = 0. The
 *            computation uses only long‑double * arithmetic and integer
 *            arithmetic on 64‑bit indices
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

static const long double LN10          = 2.3025850929940456840179914546843642076011013L;
static const long double LOG10_LN10    = 0.362183840  ;   /* log10(LN10) */

/* Return the n-th term of A106043.  */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* 3n as a long double.  */
    long double three_n = (long double)(3 * n);

    /* Compute k0 = ceil(3n − log10(3n) − log10(LN10)).  */
    long double k0_val = three_n - log10l(3.0L * n) - LOG10_LN10;
    idx_t k0 = (idx_t)ceill(k0_val);

    /* pow10 = 10^(k0−3n); the exponent is a small integer (0 or −1). */
    long double pow10 = powl(10.0L, (long double)k0 - 3.0L * n);

    /* w = ln10 * 3n * 10^(k0−3n).  */
    long double w = LN10 * three_n * pow10;

    /* m = ceil(w).  The small negative epsilon avoids rounding up
     * a number that is extremely close to an integer from below.  */
    int m = (int)ceill(w - 1e-18L);

    /* The first non‑9 digit is 10−m (always 1–8).  */
    int digit = 10 - m;
    return (seq_t)digit;
}
