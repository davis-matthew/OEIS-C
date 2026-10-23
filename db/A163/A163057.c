/*
 * A163057  An alternating sum from the n-th odd number up to the n-th odd
 *          prime.
 * https://oeis.org/A163057
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/2) * (-1 + 2*n + prime(n+1))
 * Range:     u8 n <= 53, u16 n <= 6701, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* that is, a(n) = (2*n + prime(n+1) - 1) / 2       */
/* where prime(k) is the k‑th prime (prime(1) = 2).  */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;        /* undefined for the offset, return 0 */

    /* Get the (n+1)-th prime.  get_prime(k) returns the k-th prime
       with k = 1 -> 2, 2 -> 3, etc. */
    seq_t p = (seq_t)get_prime(n + 1);

    /* Compute (2*n + p - 1) / 2 in seq_t. */
    seq_t two_n = (seq_t)n * 2;
    return (two_n + p - 1) / 2;
}
