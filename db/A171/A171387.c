/*
 * A171387  The characteristic function of primes > 3: 1 if n is prime such
 *          that neither prime+-1 is prime else 0.
 * https://oeis.org/A171387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is prime > 3 and neither n-1 nor n+1 is prime,
 *            otherwise 0
 * Range:     u8 n <= 1001, u16 n <= 1001, u32 n <= 1001, u64 n <= 1001,
 *            u128 n <= 1001, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n <= 3) {
        return 0;
    }
    if (!is_prime(n)) {
        return 0;
    }
    if (is_prime(n - 1) || is_prime(n + 1)) {
        return 0;
    }
    return 1;
}
