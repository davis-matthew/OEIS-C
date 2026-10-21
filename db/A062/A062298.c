/*
 * A062298  Number of nonprimes <= n.
 * https://oeis.org/A062298
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - pi(n) (number of nonprime integers ≤ n)
 * Range:     u8 n <= 321, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   count_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/count_primes.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;     /* not used for OEIS offset 1, but safe */
    size_t primes = count_primes(n);
    return (seq_t)n - (seq_t)primes;
}
