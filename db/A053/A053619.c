/*
 * A053619  a(n) = round(n/(log(n)-1) - pi(n)).
 * https://oeis.org/A053619
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = round(n/(log(n)-1) - pi(n))
 * Range:     i8 n <= 929, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   compute_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/compute_pi.h"

seq_t a(idx_t n)
{
    /* Safe conversion of pi(n) to a native integer type */
    idx_t pi_n = (idx_t)compute_pi(n);

    /* Compute the real‑valued expression using long double arithmetic */
    long double nd  = (long double)n;
    long double ln  = log((double)n);          /* log(·) returns double */
    long double val = nd / (ln - 1.0L) - (long double)pi_n;

    /* Round to the nearest integer (ties away from zero) */
    long long r;
    if (val >= 0.0L)
        r = (long long)(val + 0.5L);
    else
        r = (long long)(val - 0.5L);

    return (seq_t)r;
}
