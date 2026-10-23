/*
 * A103981  Number of prime factors (with multiplicity) of octahedral numbers
 *          (A005900).
 * https://oeis.org/A103981
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=0, a(1)=0; for n>1, a(n)=BigOmega((2*n^3+n)/3)
 * Range:     u8 n <= 8, u16 n <= 46, u32 n <= 1890, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bigomega_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* BigOmega is the count of prime factors (with multiplicity) of a uint64_t */
#include <stdint.h>
#include <limits.h>
#include "../utils/bigomega_uint64.h"   /* provides seq_t bigomega_uint64(uint64_t) */

/* compute a(n) */
seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;                     /* a(0)=0, a(1)=0 */

    /* compute (2*n^3 + n)/3 using seq_t arithmetic */
    seq_t N = (seq_t)n;                /* promote index to seq_t */
    seq_t val = ( (seq_t)2 * N * N * N + N ) / 3;   /* octahedral number */

    /* for n up to about 10000 the result fits in uint64_t */
    uint64_t val64 = (uint64_t)val;    /* conversion is guaranteed to be exact */

    return bigomega_uint64(val64);    /* number of prime factors with multiplicity */
}
