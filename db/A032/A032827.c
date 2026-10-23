/*
 * A032827  Numbers whose set of base-15 digits is {1,4}.
 * https://oeis.org/A032827
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 46, u32 n <= 766, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A032827: Numbers whose set of base-15 digits is {1,4}
   a(1) = 1, a(2) = 4;
   For n > 2:
       if n is odd:  a(n) = 15 * a(n / 2) + 1
       else:          a(n) = 15 * a((n - 1) / 2) + 4
   (All integer divisions truncate toward 0.)
   The function memoises computed values to guarantee O(log n) per call
   and allows arbitrary caller ordering. */

#include <stdlib.h>
#include <string.h>

/* Cached values; cache_size is the number of slots allocated. */
static seq_t *cache = NULL;
static idx_t cache_size = 0;

/* Ensure that cache has room for index n (0 <= n).  New entries are initialised to 0. */
static void ensure_cache(idx_t n)
{
    if (cache_size <= n) {
        idx_t new_size = cache_size ? cache_size * 2 : 2;
        while (new_size <= n) {
            new_size *= 2;
        }
        seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (cache) {
            memcpy(new_cache, cache, cache_size * sizeof(seq_t));
        }
        if (new_size > cache_size) {
            memset(new_cache + cache_size, 0, (new_size - cache_size) * sizeof(seq_t));
        }
        free(cache);
        cache = new_cache;
        cache_size = new_size;
    }
}

/* Recursive computation with memoisation. */
seq_t a(idx_t n)
{
    if (n == 0)          return 0;   /* behaviour below offset is irrelevant */
    if (n == 1)          return 1;
    if (n == 2)          return 4;

    ensure_cache(n);
    if (cache[n] != 0)   return cache[n];

    seq_t result;
    if (n & 1) {
        /* n is odd: a(n) = 15 * a(n / 2) + 1 */
        result = 15 * a(n >> 1) + 1;
    } else {
        /* n is even: a(n) = 15 * a((n - 1) / 2) + 4 */
        result = 15 * a((n - 1) >> 1) + 4;
    }

    cache[n] = result;
    return result;
}
