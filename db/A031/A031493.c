/*
 * A031493  Numbers whose base-9 representation has one more 0 than 8's.
 * https://oeis.org/A031493
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th positive integer whose base‑9 representation
 *            contains exactly one more digit 0 than digit 8
 * Range:     u8 n <= 43, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Checks whether a given number satisfies the condition.  */
static bool good_seq(seq_t x)
{
    int diff = 0;               /* #0s – #8s */
    seq_t y = x;
    while (y > 0) {
        int d = (int)(y % 9);
        if (d == 0) diff++;
        else if (d == 8) diff--;
        y /= 9;
    }
    return diff == 1;
}

/* Memoised storage of generated sequence values.  */
static seq_t *cache = NULL;     /* array of results */
static size_t cache_size = 0;   /* number of stored values */
static size_t cache_cap  = 0;   /* current capacity of array */
static seq_t last_tested   = 0; /* last integer examined */

/* Grows the cache array at least to the requested capacity.  */
static void grow_cache(size_t min_cap)
{
    if (min_cap <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 32;
    if (new_cap < min_cap) new_cap = min_cap;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* undefined below offset, return 0 */

    /* Ensure cache contains at least n elements.  */
    while ((idx_t)cache_size < n) {
        last_tested++;             /* check next integer */
        if (good_seq(last_tested)) {
            grow_cache(cache_size + 1);
            cache[cache_size++] = last_tested;
        }
    }
    return cache[n - 1];
}
