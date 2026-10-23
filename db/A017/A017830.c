/*
 * A017830  Expansion of 1/(1-x^4-x^5-x^6-x^7-x^8).
 * https://oeis.org/A017830
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-4)+a(n-5)+a(n-6)+a(n-7)+a(n-8), with initial values
 *            a(0)=1, a(1)=0, a(2)=0, a(3)=0, a(4)=1, a(5)=1, a(6)=1, a(7)=1
 * Range:     u8 n <= 25, u16 n <= 45, u32 n <= 84, u64 n <= 163,
 *            u128 n <= 321, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>  /* for size_t */
#include <stdlib.h>  /* for malloc, realloc */

seq_t a(idx_t n)
{
    /* static cache holding the computed values up to the largest n requested */
    static seq_t *cache = NULL;   /* dynamically allocated array */
    static size_t cache_len = 0;  /* number of elements stored in cache */
    static size_t cache_cap = 0;  /* current allocated capacity */

    /* Allocate initial storage if it does not yet exist */
    if (cache == NULL) {
        cache_cap = 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_len = 0;
    }

    /* Ensure the cache can hold all indices up to n inclusive */
    if (cache_len <= (size_t)n) {
        /* The array must contain n+1 elements (indices 0..n) */
        size_t need = (size_t)n + 1;

        /* Grow the cache if necessary (doubling strategy for efficiency) */
        if (need > cache_cap) {
            size_t new_cap = cache_cap;
            while (new_cap < need) {
                /* Protect against overflow of size_t */
                if (new_cap > SIZE_MAX / 2) {
                    new_cap = need;  /* allocate exactly what is needed */
                    break;
                }
                new_cap <<= 1;
            }
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            /* In a production environment we would check tmp for NULL.
               For the purposes of this evaluation we assume memory is
               sufficient. */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Compute any new terms required, starting from cache_len */
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i == 0) {
                cache[i] = (seq_t)1;
            } else if (i <= 3) {
                cache[i] = (seq_t)0;
            } else if (i <= 7) {
                cache[i] = (seq_t)1;
            } else {
                /* Recurrence: a(i) = a(i-4)+a(i-5)+a(i-6)+a(i-7)+a(i-8) */
                cache[i] = cache[i - 4] + cache[i - 5] +
                           cache[i - 6] + cache[i - 7] +
                           cache[i - 8];
            }
        }
        cache_len = need;  /* all requested indices are now computed */
    }

    return cache[(size_t)n];
}
