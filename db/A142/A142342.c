/*
 * A142342  a(n) = 10*prime(n).
 * https://oeis.org/A142342
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*prime(n)
 * Range:     u8 n <= 9, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* Get the n-th prime (offset 1) and multiply by 10. */
    seq_t prime = (seq_t)get_prime(n);
    return (seq_t)10 * prime;
}
