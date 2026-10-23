/*
 * A100923  a(n) = 1 iff 6*n+1 and 6*n-1 are both prime numbers (0 otherwise).
 * https://oeis.org/A100923
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 iff 6*n-1 and 6*n+1 are both prime, else 0
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   is_prime_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_uint64.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;
    }
    uint64_t p1 = 6 * n - 1;
    uint64_t p2 = 6 * n + 1;
    return (seq_t)(is_prime_uint64(p1) && is_prime_uint64(p2));
}
