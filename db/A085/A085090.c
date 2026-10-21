/*
 * A085090  If 2n-1 is prime then a(n) = 2n-1, otherwise a(n) = 0.
 * https://oeis.org/A085090
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n-1 if 2*n-1 is prime, otherwise 0
 * Range:     u8 n <= 128, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Compute the candidate prime 2*n-1 */
    uint64_t p = 2ULL * n - 1;
    if (is_prime(p))
        return (seq_t)p;
    return 0;
}
