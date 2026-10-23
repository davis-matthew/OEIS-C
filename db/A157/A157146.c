/*
 * A157146  Prime numbers containing equal number of odd prime digits and even
 *          prime digits.
 * https://oeis.org/A157146
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   a(n) = the nth prime p such that count_2(p) > 0 and
 *            count_{3,5,7}(p) = count_2(p)
 * Range:     u8 n <= 4, u16 n <= 881, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_frequencies, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* External helpers supplied by the build environment. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/next_prime.h"          /* uint64_t next_prime(uint64_t) */
#include "../utils/digit_frequencies.h"   /* void digit_frequencies(seq_t, unsigned int[10]) */

/* Returns the nth element (n >= 1) of OEIS sequence A157146. */
seq_t a(idx_t n)
{
    /* Static cache of produced terms. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;  /* number of terms already cached */
    static size_t cache_cap = 0;  /* allocated length of cache */
    static uint64_t last_checked = 1;  /* last number tested for primality */

    /* Return cached element if already available. */
    if (n <= (idx_t)cache_len)
        return cache[n - 1];

    /* Expand cache capacity if necessary before generating more terms. */
    if (cache_len >= cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
        /* Ensure new_cap is enough to hold at least the requested n. */
        if (new_cap < (size_t)n)
            new_cap = n;
        /* Reallocate cache. */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failed – return 0 as a sentinel (sequence is undefined). */
            return (seq_t)0;
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Generate primes until the cache contains at least n terms. */
    while (cache_len < (size_t)n) {
        uint64_t p = next_prime(last_checked);  /* next prime > last_checked */
        last_checked = p;

        unsigned int freq[10] = {0};
        digit_frequencies((seq_t)p, freq);

        unsigned int even_cnt  = freq[2];
        unsigned int odd_cnt   = freq[3] + freq[5] + freq[7];

        if (even_cnt > 0 && odd_cnt == even_cnt) {
            /* Ensure capacity one more element if we've reached the limit. */
            if (cache_len >= cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) {
                    return (seq_t)0;  /* Allocation failure. */
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)p;
        }
    }

    return cache[n - 1];
}
