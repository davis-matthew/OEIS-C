/*
 * A082467  Least k>0 such that n-k and n+k are both primes.
 * https://oeis.org/A082467
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k>0 such that n-k and n+k are both primes (for n
 *            >= 4)
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* The sequence is defined only for n >= 4; return 0 otherwise. */
    if (n < 4)
        return 0;

    /* Search for the minimal k > 0 with both n-k and n+k prime.
       Because n >= 4, low = n - k always stays >= 1 for k < n. */
    for (idx_t k = 1; k < n; ++k) {
        idx_t low  = n - k;
        idx_t high = n + k;
        if (is_prime(low) && is_prime(high))
            return (seq_t)k;
    }

    /* By the Goldbach conjecture (which we assume holds for the required range),
       this point should never be reached. Return 0 as a safe fallback. */
    return 0;
}
