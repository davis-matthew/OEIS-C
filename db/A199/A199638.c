/*
 * A199638  Last number in row n of triangle A199636.
 * https://oeis.org/A199638
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A199638: Last number in row n of triangle A199636.
   Recurrence for n ≥ 12:
       a(n) = 4*a(n-2) + a(n-6) - 4*a(n-8)
   Initial values (offset 4):
       a(4)=5, a(5)=3, a(6)=21, a(7)=13,
       a(8)=85, a(9)=53, a(10)=341, a(11)=227
   The function memoises intermediate results so that any order of
   calls is O(n) overall. */
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    if (n < 4) return 0;           /* values below the offset are unspecified */

    /* static data for memoisation */
    static seq_t *cache = NULL;    /* array a[0 .. cache_size]  */
    static idx_t cache_size = 0;   /* allocated size             */
    static idx_t computed_up_to = 0; /* highest index already computed */

    /* initialise on first call */
    if (cache == NULL)
    {
        cache_size = (n > 11) ? n : 11;      /* at least up to index 11 */
        cache = (seq_t *)malloc((cache_size + 1) * sizeof(seq_t));
        memset(cache, 0, (cache_size + 1) * sizeof(seq_t));

        /* set the known base values */
        cache[4] = 5;
        cache[5] = 3;
        cache[6] = 21;
        cache[7] = 13;
        cache[8] = 85;
        cache[9] = 53;
        cache[10] = 341;
        cache[11] = 227;

        computed_up_to = 11;          /* we have values up to 11 ready */
    }

    /* grow the cache if a requested index is larger than we have */
    if (n > cache_size)
    {
        seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!new_cache)             /* abort on malloc failure */
            abort();
        cache = new_cache;
        /* initialise new cells to zero (they will be set by the recurrence) */
        memset(cache + cache_size + 1, 0, (n - cache_size) * sizeof(seq_t));
        cache_size = n;
    }

    /* compute missing terms using the recurrence */
    if (n > computed_up_to)
    {
        idx_t start = (computed_up_to >= 11) ? computed_up_to + 1 : 12;
        for (idx_t i = start; i <= n; ++i)
        {
            cache[i] = 4 * cache[i - 2] + cache[i - 6] - 4 * cache[i - 8];
        }
        computed_up_to = n;
    }

    return cache[n];
}
