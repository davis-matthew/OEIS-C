/*
 * A156874  Number of Sophie Germain primes <= n.
 * https://oeis.org/A156874
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #(p ≤ n | p is prime and 2p+1 is prime)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    seq_t count = 0;
    if (n < 2) return count;   /* a(1) = 0 */

    for (idx_t p = 2; p <= n; ++p) {
        if (is_prime(p) && is_prime(2 * p + 1))
            ++count;
    }
    return count;
}
