/*
 * A109805  a(n) = prime(n+2)*prime(n+1) - prime(n)*prime(n+1).
 * https://oeis.org/A109805
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = prime(n+2)*prime(n+1) - prime(n)*prime(n+1) (offset 1)
 * Range:     u8 n <= 10, u16 n <= 325, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* For n < 1 the sequence is undefined; return 0 to avoid invalid indices. */
    if (n < 1)
        return 0;

    /* Retrieve the needed primes. */
    seq_t p_n   = get_prime((uint64_t)n);       /* prime(n)  */
    seq_t p_n1  = get_prime((uint64_t)(n + 1)); /* prime(n+1) */
    seq_t p_n2  = get_prime((uint64_t)(n + 2)); /* prime(n+2) */

    /* a(n) = prime(n+2)*prime(n+1) - prime(n)*prime(n+1) */
    return p_n2 * p_n1 - p_n * p_n1;
}
