/*
 * A175223  a(n) = prime(n) + 7.
 * https://oeis.org/A175223
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n) + 7
 * Range:     u8 n <= 53, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   int_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/int_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset is 1, below that is irrelevant */

    /* static cache for prime numbers */
    static uint64_t *primes  = NULL;
    static size_t   prime_cnt = 0;
    static size_t   prime_cap = 0;

    /* extend cache until we have at least n primes */
    if (prime_cnt < (size_t)n) {
        /* grow the underlying array if necessary */
        size_t needed = (size_t)n;
        if (prime_cap < needed) {
            size_t new_cap = prime_cap ? prime_cap << 1 : 8;
            while (new_cap < needed) new_cap <<= 1;
            uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
            primes = tmp;
            prime_cap = new_cap;
        }

        /* start searching for primes from the first missing candidate */
        uint64_t candidate = (prime_cnt == 0 ? 2ULL : primes[prime_cnt - 1] + 1ULL);
        while (prime_cnt < needed) {
            bool is_prime = true;
            uint64_t lim = int_sqrt(candidate);          /* integer sqrt helper */
            for (size_t i = 0; i < prime_cnt && primes[i] <= lim; ++i) {
                if (candidate % primes[i] == 0) {
                    is_prime = false;
                    break;
                }
            }
            if (is_prime) {
                primes[prime_cnt++] = candidate;
            }
            ++candidate;
        }
    }

    return (seq_t)primes[n - 1] + (seq_t)7;
}
