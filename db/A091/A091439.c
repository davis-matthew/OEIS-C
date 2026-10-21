/*
 * A091439  Least k such that k/phi(k) >= n, where phi is Euler's totient
 *          function.
 * https://oeis.org/A091439
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = least k such that k/phi(k) >= n (sequential search)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    /* The ratio k/phi(k) depends only on the distinct prime factors of k.
       For a set of distinct primes {p1,..,pm} the ratio is
          prod_i (pi / (pi-1)) = num / den,
       where   num = prod_i pi    (the primorial)
               den = prod_i (pi-1).
       The smallest k with a given set of primes is exactly the primorial,
       so the problem reduces to finding the smallest m for which
          num / den >= n   <=>   num >= n * den.
    */

    seq_t num = 1;      /* product of primes seen so far (primorial) */
    seq_t den = 1;      /* product of (prime-1) seen so far */

    /* n == 1 is satisfied with zero primes (k = 1). */
    if (n == 1)
        return num;

    uint64_t p = 2;      /* start with the first prime */

    while (true) {
        /* multiply by the new prime and its (p-1) factor */
        num *= (seq_t)p;
        den *= (seq_t)(p - 1);

        /* check if the ratio reaches the threshold */
        if (num >= (seq_t)n * den)
            return num;      /* k is the primorial of the primes seen */

        /* get the next prime */
        p = next_prime(p);
    }
}
