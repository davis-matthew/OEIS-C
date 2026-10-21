/*
 * A071407  Least k such that k*prime(n) + 1 and k*prime(n) - 1 are twin
 *          primes.
 * https://oeis.org/A071407
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k such that k*prime(n)−1 and k*prime(n)+1 are both
 *            prime
 * Range:     u8 n <= 5, u16 n <= 58, u32 n <= 65, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/get_prime.h"      /* extern uint32_t get_prime(idx_t k); */
#include "../utils/is_prime_big.h"   /* extern bool is_prime_big(seq_t n); */

/* Computes the n-th term of A071407.  The index n is 1‑based. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n < 1 */

    /* Retrieve the n‑th prime and cast to seq_t. */
    uint32_t p_uint = get_prime(n);
    seq_t p = (seq_t)p_uint;

    /* For p = 2 the answer is trivially 2. */
    if (p == 2) return (seq_t)2;

    /* For odd primes, only even k can give odd numbers k*p±1 that may be prime,
       because odd*odd is odd and odd±1 is even >2.  Thus we test k = 2,4,6,... */
    seq_t k = 2;
    for (;; k += 2) {
        seq_t x = k * p - (seq_t)1;
        seq_t y = k * p + (seq_t)1;
        if (is_prime_big(x) && is_prime_big(y))
            return k;
    }
}
