/*
 * A085909  Smallest prime p>prime(n) such that p+prime(n+1)-prime(n) is the
 *          next prime after p; or a(n)=0 if no such prime exists.
 * https://oeis.org/A085909
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the smallest prime p > prime(n) such that p + prime(n+1)
 *            - prime(n) is the next prime after p. If no such prime exists,
 *            a(n) = 0. (For n = 1 this is defined as 0.)
 * Range:     u8 n <= 23, u16 n <= 3076, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    /* 1‑based offset */
    if (n == 1) return (seq_t)0;

    /* n‑th prime and (n+1)‑th prime */
    uint32_t p_n   = get_prime(n);     /* prime(n)   */
    uint32_t p_np1 = get_prime(n + 1); /* prime(n+1) */

    /* Desired gap between consecutive primes */
    uint32_t diff = p_np1 - p_n;

    /* Search primes p > prime(n) having gap = diff */
    uint64_t p = next_prime((uint64_t)p_n + 1);   /* first prime > prime(n) */

    for (;;)
    {
        uint64_t q = next_prime(p + 1);          /* next prime after p */
        if ((uint32_t)(q - p) == diff)          /* gap matches diff?  */
            return (seq_t)p;                     /* smallest such p found */
        p = q;                                   /* continue searching */
    }
}
