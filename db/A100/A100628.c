/*
 * A100628  a(n) = 2^(3*prime(n) + 1).
 * https://oeis.org/A100628
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(3*prime(n) + 1) (offset 1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 12,
 *            big any n
 * Helpers:   is_prime, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Static cache for the first n primes */
    static uint64_t *primes = NULL;  /* array of found primes */
    static size_t  primes_cnt = 0;   /* number of primes stored */
    static size_t  primes_cap = 0;   /* allocated capacity */
    static uint64_t last_candidate = 1; /* last tested candidate */

    if (n == 0)
        return 0;  /* the sequence is defined for n ≥ 1 */

    /* Generate primes until we have at least n of them */
    while (primes_cnt < n) {
        uint64_t cand = (primes_cnt == 0) ? 2 : last_candidate + 1;

        /* Simple primality test: use the provided is_prime() helper */
        while (!is_prime(cand))
            cand++;

        /* Grow capacity if needed */
        if (primes_cnt >= primes_cap) {
            size_t new_cap = primes_cap ? primes_cap * 2 : 64;
            uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
            if (!tmp) exit(1);   /* allocation failed – abort */
            primes = tmp;
            primes_cap = new_cap;
        }

        primes[primes_cnt++] = cand;
        last_candidate = cand;
    }

    /* Compute the exponent: 3 * prime(n) + 1 */
    uint64_t p = primes[n - 1];
    uint64_t exp = 3 * p + 1;

    /* Return 2^exp using the provided power-of-two helper */
    return pow2((idx_t)exp);
}
