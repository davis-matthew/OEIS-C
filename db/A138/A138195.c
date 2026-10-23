/*
 * A138195  a(n) = floor(8*n/(5*log(n))).
 * https://oeis.org/A138195
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(8*n/(5*log(n)))
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The logarithm is natural (base e). */

#include <math.h>   /* log, floorl */

seq_t a(idx_t n)
{
    /* The sequence is defined only for n ≥ 2.  For smaller n the
       value is unspecified – we return 0. */
    if (n < 2)
        return 0;

    /* Use long double for extra precision.  The result fits in
       64 bits for all n up to about 10000, so we convert to an
       unsigned long long and then to seq_t. */
    const long double ln_n   = log((long double)n);
    const long double denom  = 5.0L * ln_n;
    const long double value  = (8.0L * (long double)n) / denom;

    /* Add a tiny epsilon to guard against rounding to the previous
       integer when the real value is very close to an integer. */
    const long double eps    = 1e-12L;
    unsigned long long result =
        (unsigned long long)floorl(value + eps);

    return (seq_t)result;
}
