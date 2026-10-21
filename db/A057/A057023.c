/*
 * A057023  Largest odd factor of (n-th prime-1); k when n-th prime is written
 *          as k*2^m+1 [with k odd].
 * https://oeis.org/A057023
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = largest odd factor of (p_n - 1), where p_n is the nth
 *            prime
 * Range:     u8 n <= 54, u16 n <= 7555, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* Offset is 1, but values for n < 1 are undefined; return 0. */
    if (n < 1) return (seq_t)0;

    /* Retrieve the nth prime (uint32_t) and cast to seq_t. */
    seq_t val = (seq_t)get_prime(n);
    val = val - (seq_t)1;            /* p_n - 1 */

    /* Strip powers of two to leave the largest odd factor. */
    while ((val & (seq_t)1) == (seq_t)0)
        val >>= 1;

    return val;
}
