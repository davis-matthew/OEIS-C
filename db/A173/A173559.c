/*
 * A173559  a(n)= +2*a(n-2) +4*a(n-3), n>3.
 * https://oeis.org/A173559
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 13, i32 n <= 29, i64 n <= 29, i128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A173559
 *   a(n) = 2*a(n-2) + 4*a(n-3)   for n > 3
 *   a(0) = 1
 *   a(1) = -6
 *   a(2) = -13
 *   a(3) = -27
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static bool initialized = false;

    /* helper to grow cache to at least n+1 elements */
    /* n is unsigned, avoid overflow in loop */
    {
        if (cache_len <= n) {
            idx_t new_len = cache_len ? cache_len : 1;
            while (new_len <= n) new_len <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * (new_len + 1));
            /* If reallocation failed, abort (sequence build could not continue) */
            if (!tmp) abort();
            cache = tmp;
            cache_len = new_len;
        }
    }

    /* initialise base values only once */
    if (!initialized) {
        if (cache_len < 3) {
            /* ensure capacity for indices 0..3 */
            idx_t new_len = 3;
            while (new_len <= 3) new_len <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * (new_len + 1));
            if (!tmp) abort();
            cache = tmp;
            cache_len = new_len;
        }
        cache[0] = 1;
        cache[1] = -6;
        cache[2] = -13;
        cache[3] = -27;
        initialized = true;
    }

    /* compute missing values up to n */
    if (n > 3) {
        for (idx_t i = 4; i <= n; ++i) {
            /* use temporary variables to avoid accidental overflow beyond needed */
            seq_t v1 = cache[i - 2];
            seq_t v2 = cache[i - 3];
            cache[i] = (seq_t)(2 * v1 + 4 * v2);
        }
    }

    return cache[n];
}
