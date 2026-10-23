/*
 * A045791  Squares with initial digit '7'.
 * https://oeis.org/A045791
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A045861(n)^2 (Squares whose decimal expansion starts with
 *            digit 7)
 * Range:     u8 none, u16 n <= 8, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* stored squares with leading digit 7   */
static size_t cache_cap = 0;         /* capacity of cache array              */
static size_t cache_len = 0;         /* number of elements currently stored  */
static idx_t next_t = 1;             /* next integer to test for leading 7  */

seq_t a(idx_t n)
{
    /* If we already computed this term before, return it immediately. */
    if (n <= cache_len && n >= 1)
        return cache[n-1];

    /* Extend the cache until we have computed at least n terms. */
    while (cache_len < n)
    {
        /* Test next integer t for having square with leading digit 7. */
        seq_t t = (seq_t)next_t;
        ++next_t;

        seq_t s = t * t;          /* square of t */
        seq_t x = s;
        while (x >= 10)           /* remove lower digits until only the first remains */
            x /= 10;
        if (x == 7)               /* leading digit is 7 */
        {
            /* Ensure capacity for one more element. */
            if (cache_len == cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!new_ptr) {   /* allocation failed – return 0 as a safe value */
                    if (cache) free(cache);
                    cache = NULL;
                    cache_cap = cache_len = 0;
                    return 0;
                }
                cache = new_ptr;
                cache_cap = new_cap;
            }
            cache[cache_len++] = s;   /* store the square */
        }
    }

    return cache[n-1];
}
