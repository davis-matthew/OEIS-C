/*
 * A138691  Numbers of the form 68+p^2 (where p is a prime).
 * https://oeis.org/A138691
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 68 + p_n^2 where p_n is the n-th prime
 * Range:     u8 n <= 6, u16 n <= 54, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0; /* undefined for n<1, return 0 */

    /* static cache of primes */
    static uint64_t *primes = NULL;   /* array of primes */
    static size_t prime_cnt = 0;      /* number of primes stored */
    static size_t prime_cap = 0;      /* allocated capacity */
    static uint64_t last_prime = 1;   /* last prime found (1 -> next is 2) */

    /* enlarge cache if necessary */
    if (prime_cnt < (size_t)n) {
        size_t new_cap = prime_cap ? prime_cap * 2 : 8;
        if (new_cap < (size_t)n) new_cap = n;

        uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
        if (!tmp) return 0;                 /* allocation failure */
        primes = tmp;
        prime_cap = new_cap;

        /* generate missing primes */
        for (size_t i = prime_cnt; i < new_cap; ++i) {
            last_prime = next_prime(last_prime);
            primes[i] = last_prime;
        }
        prime_cnt = new_cap;
    }

    uint64_t p = primes[n - 1];
    seq_t res = (seq_t)68 + (seq_t)p * (seq_t)p;
    return res;
}
