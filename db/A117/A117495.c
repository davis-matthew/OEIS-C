/*
 * A117495  Product of a prime number p and the number of primes smaller than
 *          p.
 * https://oeis.org/A117495
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1)*prime(n) (offset 1)
 * Range:     u8 n <= 9, u16 n <= 110, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A117495: product of the nth prime and the number of primes < it */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* offset is 1, n=0 not defined */
    uint32_t p = get_prime(n);                /* nth prime (fits in 32 bits) */
    /* cast n-1 to seq_t to avoid unsigned underflow for n>=1 */
    seq_t result = (seq_t)(n - 1) * (seq_t)p;
    return result;
}
