/*
 * A124127  Numbers k such that 17k + 1 is prime.
 * https://oeis.org/A124127
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 380, u32 n <= 381, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for the n-th integer k such that 17*k + 1 is prime (OEIS A124127) */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /*  OEIS offset: a(1) is the first term listed.  */
    if (n == (idx_t)0)
        return (seq_t)0;   /* undefined for offset < 1 */

    /* Static state retained between calls */
    static seq_t *cache = NULL;       /* cached k values */
    static size_t cache_len = 0;      /* number of stored values */
    static size_t cache_cap = 0;      /* allocated capacity */
    static seq_t last_k_scanned = 0;  /* last candidate checked */

    /* Ensure the cache is large enough for the requested n */
    if (cache_cap < (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        while (new_cap < (size_t)n)
            new_cap *= 2;
        /* realloc is safe: seq_t may be a struct or a scalar */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Extend the cache until it contains the n-th term */
    while (cache_len < (size_t)n) {
        seq_t k = last_k_scanned + (seq_t)1;
        last_k_scanned = k;
        seq_t p = ((seq_t)17 * k) + (seq_t)1;
        if (is_prime_big(p)) {
            cache[cache_len++] = k;
        }
    }

    return cache[(size_t)n - 1];
}
