/*
 * A229347  a(1) = 1, for n > 1 a(n) = n*2^(omega(n)-1) where omega is
 *          A001221.
 * https://oeis.org/A229347
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; for n > 1, a(n) = n*2^(omega(n)-1), where omega(n) is
 *            the number of distinct prime factors of n
 * Range:     u8 n <= 65, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   count_distinct_primes, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/count_distinct_primes.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;

    /* omega(n) = number of distinct prime factors, at least 1 for n>1 */
    int omega = count_distinct_primes((uint64_t)n);

    /* Compute 2^(omega-1) as a seq_t, then multiply by n */
    seq_t factor = pow2((idx_t)(omega - 1));
    return (seq_t)n * factor;
}
