/*
 * A210497  a(n) = 2*prime(n+1) - prime(n).
 * https://oeis.org/A210497
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*prime(n+1) - prime(n)
 * Range:     u8 n <= 52, u16 n <= 6539, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* a(1) is the first term of the sequence; values for n<1 are undefined. */
    if (n == 0) {
        return 0;
    }

    /* Compute 2*prime(n+1) and subtract prime(n).  The intermediate
       arithmetic is performed in 64‑bit (uint64_t) to avoid overflow
       for all indices that reasonably fit in seq_t. */
    return (seq_t)((2ULL * get_prime(n + 1)) - get_prime(n));
}
