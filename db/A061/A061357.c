/*
 * A061357  Number of 0<k<n such that n-k and n+k are both primes.
 * https://oeis.org/A061357
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=1}^{n-1} [is_prime(n-k) && is_prime(n+k)]
 * Range:     u8 n <= 3464, u16 n <= 8849, u32 n <= 8849, u64 n <= 8849,
 *            u128 n <= 8849, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* a(0) is undefined; return 0 to be safe. */
    if (n == 0)
        return 0;

    seq_t count = 0;
    for (idx_t k = 1; k < n; ++k) {
        if (is_prime(n - k) && is_prime(n + k))
            ++count;
    }
    return count;
}
