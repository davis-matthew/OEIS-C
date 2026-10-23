/*
 * A158611  0, 1 and the primes.
 * https://oeis.org/A158611
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A008578(n-1) = 0, 1, then the primes in order
 * Range:     u8 n <= 56, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;

    idx_t target   = n - 2;          /* how many primes to skip */
    idx_t count    = 0;
    idx_t candidate = 2;             /* start searching primes from 2 */

    for (;;) {
        if (is_prime(candidate)) {
            ++count;
            if (count == target) return (seq_t)candidate;
        }
        ++candidate;
    }
}
