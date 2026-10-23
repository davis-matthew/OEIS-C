/*
 * A159477  a(n) = smallest prime >= n, if 1 is counted as a prime.
 * https://oeis.org/A159477
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest prime >= n, with 1 counted as a prime
 * Range:     u8 n <= 251, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;          /* a(1) = 1 and a(0) not used */

    uint64_t x = (uint64_t)n;     /* idx_t is uint64_t per spec */

    if (is_prime(x))
        return (seq_t)x;          /* n itself is prime */

    /* Increment until we find the next prime. */
    for (;; ++x)
    {
        if (is_prime(x))
            return (seq_t)x;
    }
}
