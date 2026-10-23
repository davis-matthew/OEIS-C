/*
 * A032628  Primes that are concatenations of n with n + 5.
 * https://oeis.org/A032628
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth prime of the form concat(m, m+5) for integer m ≥
 *            1
 * Range:     u8 none, u16 n <= 10, u32 n <= 10, u64 n <= 600, u128 n <= 600,
 *            big any n
 * Helpers:   concat_parts, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/concat_parts.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;             /* offset 1 – undefined for n = 0 */

    /* Static cache of found primes */
    static seq_t *cache   = NULL;      /* array of primes */
    static size_t cache_len = 0;       /* number of primes stored */
    static size_t cache_cap = 0;       /* allocated capacity */
    static idx_t next_m   = 1;         /* next candidate m to test */

    /* If already computed up to n, return cached value */
    if (cache_len >= n) {
        return cache[n - 1];
    }

    /* Ensure sufficient capacity for at least one more entry */
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();   /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Scan consecutive m until we have found n primes */
    for (; cache_len < n; ++next_m) {
        seq_t cand = concat_parts(next_m, next_m + 5);
        if (is_prime_big(cand)) {
            cache[cache_len++] = cand;

            /* If buffer fills during the loop, double its capacity */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
                cache_cap = new_cap;
            }
        }
    }

    return cache[n - 1];
}
