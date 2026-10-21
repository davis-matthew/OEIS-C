/*
 * A231483  Smallest k > A082467(n) such that n-k and n+k are both prime.
 * https://oeis.org/A231483
 *
 * Offset:    20    (a(20) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the smallest k > A082467(n) such that n-k and n+k are
 *            primes
 * Range:     u8 n <= 1870, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    /* Search for the first k with n-k and n+k prime (A082467(n)). */
    idx_t k;
    idx_t first_k = 0;
    if (n < 3) return (seq_t)0;          /* not relevant for n >= 20, but safe */
    for (k = 1; k <= n - 2; ++k) {
        if (is_prime(n - k) && is_prime(n + k)) {
            first_k = k;
            break;
        }
    }
    if (first_k == 0) return (seq_t)0;   /* no such k, should not occur for n >= 20 */

    /* Find the next k > first_k satisfying the same property. */
    for (k = first_k + 1; k <= n - 2; ++k) {
        if (is_prime(n - k) && is_prime(n + k)) {
            return (seq_t)k;
        }
    }
    return (seq_t)0;   /* no second k found, this case should not occur for n >= 20 */
}
