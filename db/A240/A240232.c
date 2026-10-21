/*
 * A240232  a(n) is the smallest positive integer such that prime(n) + a(n)
 *          and prime(n) + a(n)^2 are both prime numbers.
 * https://oeis.org/A240232
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 2703, u32 n <= 3681, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   This function implements the OEIS definition:
   for n >= 1, a(n) is the smallest positive integer x such that
   prime(n) + x and prime(n) + x^2 are both prime.
*/
#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;                      /* undefined for n < 1 */

    /* prime(n) fits in 32‑bit because the first 10 000 primes are < 2³¹ */
    uint32_t p = get_prime(n);

    unsigned long long x = 1;                 /* candidate for a(n) */
    for (;; ++x) {
        unsigned long long p1 = (unsigned long long)p + x;
        unsigned long long p2 = (unsigned long long)p + x * x;

        if (is_prime_big((seq_t)p1) && is_prime_big((seq_t)p2))
            return (seq_t)x;
    }
}
