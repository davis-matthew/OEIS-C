/*
 * A221490  Number of primes of the form k*n + k - n, 1 <= k <= n.
 * https://oeis.org/A221490
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of primes of the form k*n + k - n for 1 <= k <= n
 * Range:     u8 n <= 16, u16 n <= 377, u32 n <= 377, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    seq_t count = 0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t val = (seq_t)k * ((seq_t)n + 1);
        val -= (seq_t)n;          /* val = k*n + k - n = k*(n+1) - n */
        if (is_prime_big(val))
            ++count;
    }
    return count;
}
