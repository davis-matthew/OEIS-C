/*
 * A062469  Integer part of log(n^n)^(n^(-1/n)).
 * https://oeis.org/A062469
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n*log(n))^(n^(-1/n)))
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;           /* log(1^1)=0, 0^1 = 0 */

    /* all integer arithmetic done via seq_t */
    long double nd   = (long double)n;
    long double ln_n = logl(nd);                /* ln(n) */
    long double L    = nd * ln_n;               /* n * ln(n) = log(n^n) */

    /* n^(-1/n) = exp(-ln(n)/n) */
    long double e    = expl(-ln_n / nd);

    /* (n*log n)^e = exp(e * ln(L)) */
    long double val  = expl(e * logl(L));

    /* convert to integer using a small epsilon to avoid boundary errors */
    const long double eps = 1e-12L;
    unsigned long long res = (unsigned long long)(floorl(val + eps));

    return (seq_t)res;
}
