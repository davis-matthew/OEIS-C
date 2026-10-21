/*
 * A050501  a(n) = ceiling(n/log(n)).
 * https://oeis.org/A050501
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil(n / log(n))
 * Range:     u8 n <= 1929, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 2; below that the result is irrelevant. */
    if (n < 2) return 0;

    /* Work with doubles only for approximating the real logarithm.  The
       integer result is kept in an unsigned long long to avoid constructing
       a seq_t from a floating point value. */
    double dn = (double)n;
    double ln = log(dn);               /* natural logarithm of n */

    unsigned long long k = (unsigned long long)(dn / ln); /* floor(n/ln(n)) */
    if (k == 0) k = 1;                 /* protect against tiny n */

    /* Move k upward until k*ln(n) > n.  Then move downward while still
       satisfying the inequality, to guarantee minimality. */
    while ((double)k * ln <= dn) ++k;
    while (k > 1 && (double)(k - 1) * ln > dn) --k;

    return (seq_t)k;
}
