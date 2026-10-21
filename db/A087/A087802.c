/*
 * A087802  a(n) = Sum_{d|n, d nonprime} mu(d), where mu = A008683.
 * https://oeis.org/A087802
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = if n=1 then 1 else A001221(n) (the number of distinct
 *            prime factors of n)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   count_distinct_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/count_distinct_primes.h"

seq_t a(idx_t n)
{
    if (n == 1) {
        return (seq_t)1;
    }
    return (seq_t)count_distinct_primes((uint64_t)n);
}
