/*
 * A173956  n-th primorial modulo n.
 * https://oeis.org/A173956
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (product of first n primes) mod n
 * Range:     u8 n <= 62, u16 n <= 7127, u32 n <= 9897, u64 n <= 9897,
 *            u128 n <= 9897, big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* should not be called with n=0 */

    /* static cache of the first primes encountered so far */
    static uint64_t *primes = NULL;
    static size_t primes_cnt = 0;
    static uint64_t last_prime = 1;  /* start just before the first prime 2 */

    /* grow the prime list until it contains at least n primes */
    if (primes_cnt < (size_t)n) {
        size_t new_cnt = (size_t)n;
        primes = (uint64_t *)realloc(primes, new_cnt * sizeof(uint64_t));
        for (size_t i = primes_cnt; i < new_cnt; ++i) {
            last_prime = next_prime(last_prime);
            primes[i] = last_prime;
        }
        primes_cnt = new_cnt;
    }

    /* compute primorial modulo n */
    seq_t prod = 1;
    seq_t mod  = (seq_t)n;
    for (size_t i = 0; i < (size_t)n; ++i) {
        prod = (prod * (seq_t)primes[i]) % mod;
    }
    return prod;
}
