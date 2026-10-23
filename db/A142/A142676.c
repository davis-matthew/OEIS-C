/*
 * A142676  Primes congruent to 17 mod 57.
 * https://oeis.org/A142676
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 184, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes ≡ 17 (mod 57)  (OEIS A142676)
 *
 * The function `a(n)` returns the nth prime congruent to 17 modulo 57,
 * where n starts at 1.  It lazily builds a sieve up to a limit until
 * enough such primes have been found, and caches the result for future
 * calls.  The algorithm scales comfortably up to n ≈ 10 000 in well
 * under 2 s on typical hardware.
 */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* static cache */
    static idx_t cache_limit = 0;      /* last used sieve limit           */
    static uint64_t *cache_arr = NULL; /* array of primes ≡17 mod 57      */
    static idx_t  cache_size = 0;      /* number of elements in cache_arr*/

    /* rebuild cache if necessary */
    if (cache_arr == NULL || n > cache_size) {
        /* start with a moderate upper bound */
        idx_t limit = (cache_limit > 0) ? cache_limit : 1000;
        /* increase until we have at least n entries */
        while (1) {
            /* allocate boolean array for sieve */
            bool *is_prime = (bool *)malloc((limit + 1) * sizeof(bool));
            if (!is_prime) return 0;               /* allocation failure */
            memset(is_prime, 1, (limit + 1) * sizeof(bool));
            is_prime[0] = is_prime[1] = false;

            /* classic Eratosthenes */
            for (idx_t p = 2; p <= limit / p; ++p) {
                if (is_prime[p]) {
                    for (idx_t q = p * p; q <= limit; q += p)
                        is_prime[q] = false;
                }
            }

            /* count how many primes ≡17 (mod 57) */
            idx_t count = 0;
            for (idx_t i = 2; i <= limit; ++i)
                if (is_prime[i] && (i % 57u == 17u))
                    ++count;

            /* if enough, collect them */
            if (count >= n) {
                uint64_t *new_arr = (uint64_t *)malloc(count * sizeof(uint64_t));
                if (!new_arr) {
                    free(is_prime);
                    return 0; /* allocation failure */
                }
                idx_t idx = 0;
                for (idx_t i = 2; i <= limit; ++i)
                    if (is_prime[i] && (i % 57u == 17u))
                        new_arr[idx++] = i;

                /* free old cache */
                free(cache_arr);
                cache_arr = new_arr;
                cache_size = count;
                cache_limit = limit;
                free(is_prime);
                break;
            }

            /* not enough, free and try a larger limit */
            free(is_prime);
            free(cache_arr);
            cache_arr = NULL;
            cache_size = 0;
            /* double the limit to get more primes */
            limit *= 2;
        }
    }

    return (seq_t)cache_arr[n - 1];
}
