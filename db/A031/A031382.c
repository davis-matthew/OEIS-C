/*
 * A031382  a(n) = prime(8*n - 2).
 * https://oeis.org/A031382
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(8*n - 2)
 * Range:     u8 n <= 7, u16 n <= 818, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* The OEIS offset is 1; for n = 0 the value is undefined,
       so return 0 in that case. */
    if (n == 0)
        return 0;

    /* Compute the index for the desired prime.  Using idx_t arithmetic
       avoids overflow even for very large n. */
    idx_t k = (idx_t)n * 8 - 2;

    /* Return the k‑th prime, cast to seq_t. */
    return (seq_t)get_prime(k);
}
