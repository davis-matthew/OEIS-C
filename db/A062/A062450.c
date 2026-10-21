/*
 * A062450  Nearest integer to log(n^n)^(1 + log(1 + log(n))).
 * https://oeis.org/A062450
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nearest integer to (log(n^n))^(1 + log(1 + log(n)))
 * Range:     u8 n <= 7, u16 n <= 27, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* that is a(n) = round( (n*ln(n)) ^ (1 + ln(1 + ln(n))) ) for n >= 2,
   and a(0) = a(1) = 0. This uses only standard math functions and
   integer arithmetic on seq_t. */
#include <math.h>

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;

    const double dn = (double)n;
    const double ln_n = log(dn);
    const double base = dn * ln_n;                  /* n * ln(n) */
    const double ln1lnn = log(1.0 + ln_n);          /* ln(1 + ln n) */
    const double exponent = 1.0 + ln1lnn;           /* 1 + ln(1 + ln n) */
    const double val = pow(base, exponent);         /* (n ln n)^(1+ln(1+ln n)) */
    const long long rounded = (long long)floor(val + 0.5); /* nearest integer */
    return (seq_t)rounded;
}
