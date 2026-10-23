/*
 * A190306  a(n) = n + [n*r/t] + [n*s/t]; r=1, s=csch(1), t=sech(1).
 * https://oeis.org/A190306
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(n*cosh(1)) + floor(n*coth(1))
 * Range:     u8 n <= 66, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

static const double COSH1 = 1.5430806348152437;   /* cosh(1) */
static const double COTH1 = 1.3130352854993315;   /* coth(1) */
static const double EPS   = 1e-12;               /* safety margin for floor */

/* External linkage function for OEIS sequence A190306 */
seq_t a(idx_t n)
{
    /* n fits in uint64_t; the sequence values are small for the tested range */
    uint64_t n64 = (uint64_t)n;

    /* compute the floor terms using double arithmetic with a safety epsilon */
    uint64_t f1 = (uint64_t)floor((double)n64 * COSH1 + EPS);
    uint64_t f2 = (uint64_t)floor((double)n64 * COTH1 + EPS);

    /* convert each part to seq_t and add them */
    seq_t result = (seq_t)n64 + (seq_t)f1 + (seq_t)f2;
    return result;
}
