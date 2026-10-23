/*
 * A181591  a(n) = binomial(bigOmega(n),omega(n)), where omega = A001221 and
 *          bigOmega = A001222.
 * https://oeis.org/A181591
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(bigOmega(n), omega(n))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bigomega, binom, count_distinct_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* bigOmega(n) = total number of prime factors of n with multiplicity (A001222) */
/* omega(n)    = number of distinct prime factors of n          (A001221) */

#include <stdint.h>
#include "../utils/bigomega.h"
#include "../utils/count_distinct_primes.h"
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1; for n = 0 return 0. */
    if (n == 0)
        return (seq_t)0;

    /* bigOmega is the total number of prime factors with multiplicity. */
    seq_t bigOmega = bigomega(n);

    /* omega is the number of distinct prime factors. */
    int omega = count_distinct_primes((uint64_t)n);

    /* Compute binomial(bigOmega, omega). The binom routine expects idx_t
       arguments; cast appropriately.  For typical n < 2^64 the conversion
       is safe. */
    return binom((idx_t)bigOmega, (idx_t)omega);
}
