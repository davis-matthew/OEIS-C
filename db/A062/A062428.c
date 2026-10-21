/*
 * A062428  Nearest integer to log(n^n)^(1 - 1/n).
 * https://oeis.org/A062428
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nearest integer to (log(n^n))^(1 - 1/n) = nearest integer
 *            to (n*log(n))^(1 - 1/n)
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    if (n <= 1)   /* a(1) = 1 by definition */
        return (seq_t)1;

    double nd = (double)n;
    double ln = log(nd);                      /* natural log */
    double base = nd * ln;                     /* log(n^n) */
    double exp  = 1.0 - 1.0 / nd;             /* 1 - 1/n */
    double val  = pow(base, exp);             /* (n*log(n))^(1-1/n) */

    /* round to nearest integer */
    long long rounded = (long long)floor(val + 0.5 + 1e-12);

    return (seq_t)rounded;
}
