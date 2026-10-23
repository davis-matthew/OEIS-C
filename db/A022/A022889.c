/*
 * A022889  n-th prime p(k) such that p(k) + p(k+5) = p(k+1) + p(k+4).
 * https://oeis.org/A022889
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th prime p(k) such that p(k) + p(k+5) = p(k+1) + p(k+4)
 * Range:     u8 n <= 14, u16 n <= 501, u32 n <= 501, u64 n <= 501,
 *            u128 n <= 501, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

seq_t a(idx_t n)
{
    /* Static cache of computed terms and the limit up to which we have sieved */
    static idx_t sieve_limit = 0;          /* upper bound that has been sieved */
    static seq_t *cache = NULL;            /* array of found terms */
    static size_t cache_cap = 0;           /* capacity of cache */
    static size_t cache_len = 0;           /* number of terms stored */

    /* Grow the cache when needed */
    if (cache_cap == 0) {
        cache_cap = 1024;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;
    }

    /* Repeatedly sieve larger ranges until we have enough terms */
    while (cache_len < n) {
        /* Choose a new upper bound for the sieve */
        idx_t new_limit = sieve_limit == 0 ? 1000 : sieve_limit * 2;
        if (new_limit < 1000) new_limit = 1000;

        /* Allocate and initialise the sieve array */
        size_t size = (size_t)new_limit + 1;
        char *sieve = (char *)malloc(size);
        if (!sieve) return (seq_t)0;
        memset(sieve, 1, size);
        sieve[0] = sieve[1] = 0;

        /* Eratosthenes marking */
        size_t sq = (size_t)sqrt((double)new_limit);
        for (size_t p = 2; p <= sq; ++p) {
            if (sieve[p]) {
                for (size_t j = p * p; j <= (size_t)new_limit; j += p)
                    sieve[j] = 0;
            }
        }

        /* Sliding window of the last six primes */
        uint32_t window[6] = {0};
        size_t prime_cnt = 0;              /* number of primes seen so far */
        cache_len = 0;                    /* reset cache for this range */

        /* Scan the sieve and process primes */
        for (size_t i = 2; i <= (size_t)new_limit; ++i) {
            if (!sieve[i]) continue;     /* not prime */

            /* Shift window and insert new prime */
            for (int k = 0; k < 5; ++k) window[k] = window[k + 1];
            window[5] = (uint32_t)i;

            if (++prime_cnt < 6) continue; /* need at least six primes */

            /* Condition: p(k)+p(k+5) == p(k+1)+p(k+4) */
            int64_t d1 = (int64_t)window[0] - (int64_t)window[1];
            int64_t d2 = (int64_t)window[4] - (int64_t)window[5];
            if (d1 == d2) {
                /* Append the qualifying prime to the cache */
                if (cache_len == cache_cap) {
                    size_t new_cap = cache_cap * 2 + 10;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp) {
                        free(sieve);
                        return (seq_t)0;
                    }
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = (seq_t)window[0];
            }
        }

        free(sieve);
        sieve_limit = new_limit;
    }

    return cache[n - 1];
}
