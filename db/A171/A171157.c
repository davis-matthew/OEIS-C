/*
 * A171157  Number of distinct primes > 3 that divide n.
 * https://oeis.org/A171157
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = count_distinct_primes(n) - I(n % 2 == 0) - I(n % 3 == 0)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_distinct_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where I(condition) is 1 if condition is true, otherwise 0. */
/* It counts distinct prime divisors of n, excluding 2 and 3. */

#include <stdint.h>
#include "../utils/count_distinct_primes.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* safety for index 0, not used by OEIS offset */
    seq_t cnt = (seq_t)count_distinct_primes((uint64_t)n);

    /* Subtract 1 if 2 is a prime divisor */
    if (((uint64_t)n % 2ULL) == 0ULL) {
        if (cnt > 0) cnt--;
    }

    /* Subtract 1 if 3 is a prime divisor */
    if (((uint64_t)n % 3ULL) == 0ULL) {
        if (cnt > 0) cnt--;
    }

    return cnt;
}
