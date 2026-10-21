/*
 * A098751  a(n+1) = a(n) + 10's complement of each of the digits of a(n);
 *          a(0) = 0.
 * https://oeis.org/A098751
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = a(n-1) + Σ(10 - d), d=digits of a(n-1), a(0)=0
 * Range:     u8 n <= 17, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* Number of elements computed so far */
    static idx_t cache_cap = 0;   /* Allocated capacity */

    /* Ensure capacity up to n */
    if (n >= cache_len) {
        /* Determine new capacity */
        idx_t needed = n + 1;
        if (cache_cap == 0) cache_cap = 10;
        while (cache_cap < needed) {
            cache_cap *= 2;
        }

        /* Allocate or reallocate */
        seq_t *new_cache;
        if (cache == NULL) {
            new_cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        } else {
            new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * cache_cap);
        }
        if (new_cache == NULL) {
            /* Out of memory: return 0 (sequence cannot be computed) */
            return 0;
        }
        cache = new_cache;

        /* Compute new entries */
        if (cache_len == 0) {
            /* Initialize a(0) */
            cache[0] = 0;
            cache_len = 1;
        }
        for (idx_t k = cache_len; k <= n; ++k) {
            seq_t prev = cache[k - 1];
            /* Compute digit count and digit sum */
            seq_t temp = prev;
            unsigned long long digit_sum = 0;
            unsigned long long digit_count = (temp == 0) ? 1ULL : 0ULL;
            while (temp != 0) {
                seq_t d = temp % 10;
                digit_sum += (unsigned long long)d;
                digit_count++;
                temp /= 10;
            }
            /* Increment: 10*digit_count - digit_sum */
            seq_t inc = 10 * (seq_t)digit_count - (seq_t)digit_sum;
            cache[k] = prev + inc;
        }
        cache_len = n + 1;
    }

    return cache[n];
}
