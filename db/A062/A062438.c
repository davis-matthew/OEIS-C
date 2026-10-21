/*
 * A062438  Nearest integer to log(n^n)^log(log(1 + n)).
 * https://oeis.org/A062438
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nearest integer to (n*log(n))^(log(log(n+1)))
 * Range:     u8 n <= 28, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  For n < 2 the behavior is undefined; the function returns 0.   */
#include <math.h>

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                    /* undefined for n < 2 */

    /* Compute using long double for extra precision. */
    const long double ln_n      = logl((long double)n);          /* ln n           */
    const long double base      = (long double)n * ln_n;         /* n · ln n       */
    const long double ln_n1     = logl((long double)(n + 1));     /* ln (n+1)       */
    const long double exponent  = logl(ln_n1);                    /* ln ln (n+1)    */

    /* (base)^(exponent) = exp( exponent * ln(base) ) */
    const long double log_val   = exponent * logl(base);
    const long double val       = expl(log_val);

    /* Round to nearest integer and convert to the integer type `seq_t`.   */
    const unsigned long long rounded = (unsigned long long)(val + 0.5L);
    return (seq_t)rounded;
}
