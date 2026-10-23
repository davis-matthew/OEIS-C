/*
 * A014690  a(n) = n + prime(n+1).
 * https://oeis.org/A014690
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + prime(n+1)
 * Range:     u8 n <= 45, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* Get the (n+1)-th prime.  The helper returns a uint32_t. */
    seq_t p = (seq_t)get_prime((idx_t)(n + 1));
    /* Return n + prime(n+1). */
    return (seq_t)n + p;
}
