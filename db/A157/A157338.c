/*
 * A157338  First primes in successive prime centuries.
 * https://oeis.org/A157338
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = first prime p such that p >= 100*(n-1)+1
 * Range:     u8 n <= 3, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;           // undefined for n = 0, return a default
    idx_t cur = 100ULL * (n - 1ULL) + 1ULL;
    while (!is_prime(cur)) {
        ++cur;
    }
    return (seq_t)cur;
}
