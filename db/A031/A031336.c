/*
 * A031336  a(n) = prime(3*n).
 * https://oeis.org/A031336
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(3*n)
 * Range:     u8 n <= 18, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* OEIS offset is 1; undefined behaviour for n ≤ 0 is allowed,
       but we return 0 for safety. */
    if (n == 0)
        return 0;

    /* Compute the index of the desired prime: the 3*n-th prime. */
    idx_t idx = 3 * n;

    /* Retrieve the prime using the utility function. */
    uint32_t p = get_prime(idx);

    /* Convert to seq_t (truncated if necessary; seq_t is at least 32‑bit). */
    return (seq_t)p;
}
