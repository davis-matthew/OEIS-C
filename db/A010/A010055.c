/*
 * A010055  1 if n is a prime power p^k (k >= 0), otherwise 0.
 * https://oeis.org/A010055
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if A001221(n) ≤ 1 else 0; i.e. if n is a prime power or
 *            1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_distinct_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/count_distinct_primes.h"

seq_t a(idx_t n)
{
    /* Handle the unit case */
    if (n == 1) return (seq_t)1;

    /* Count distinct prime factors of n */
    uint64_t x = (uint64_t)n;
    if (count_distinct_primes(x) <= 1) return (seq_t)1;
    return (seq_t)0;
}
