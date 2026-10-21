/*
 * A220542  Equals one maps: number of n X 2 binary arrays indicating the
 *          locations of corresponding elements equal to exactly one of their
 *          horizontal and vertical neighbors in a random 0..2 n X 2 array.
 * https://oeis.org/A220542
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - 7*a(n-2) - 6*a(n-3) + 7*a(n-4) + 4*a(n-5) *
 *            for n>10. Initial terms: a(1)=2, a(2)=8, a(3)=38, a(4)=176, *
 *            a(5)=816, a(6)=3616, a(7)=15362, a(8)=63484, a(9)=258180, *
 *            a(10)=1041120
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_len = 0;

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;               /* undefined offset */

    /* initialise cache with the first 10 terms (index 0 unused) */
    if (cache == NULL) {
        cache_len = 11;                        /* indices 0..10 */
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_len);
        if (cache == NULL) exit(1);            /* allocation failure */

        cache[0] = 0;
        cache[1] = 2;
        cache[2] = 8;
        cache[3] = 38;
        cache[4] = 176;
        cache[5] = 816;
        cache[6] = 3616;
        cache[7] = 15362;
        cache[8] = 63484;
        cache[9] = 258180;
        cache[10] = 1041120;
    }

    /* if n is already cached, return instantly */
    if ((size_t)n < cache_len) return cache[(size_t)n];

    /* grow the cache to hold up to index n */
    size_t new_len = (size_t)n + 1;
    if (new_len > cache_len) {
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_len);
        if (cache == NULL) exit(1);            /* allocation failure */

        /* compute missing terms using the recurrence */
        for (idx_t i = (idx_t)cache_len; i <= n; ++i) {
            seq_t next = 6 * cache[i-1] - 7 * cache[i-2]
                      - 6 * cache[i-3] + 7 * cache[i-4]
                      + 4 * cache[i-5];
            cache[i] = next;
        }
        cache_len = new_len;
    }

    return cache[(size_t)n];
}
