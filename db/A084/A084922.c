/*
 * A084922  a(n) = (prime(n)-1)*(prime(n)+1)/6.
 * https://oeis.org/A084922
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (prime(n)-1)*(prime(n)+1)/6
 * Range:     u8 n <= 6, u16 n <= 102, u32 n <= 840, u64 n <= 840,
 *            u128 n <= 840, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* for n >= 3, get the n-th prime */
    uint32_t p = get_prime(n);   /* prime(n) fits in 32 bits for n up to 10000 */
    seq_t prod = (seq_t)(p - 1) * (seq_t)(p + 1);
    return prod / 6;
}
