/*
 * A185818  1/5 the number of n X 2 0..4 arrays with every element equal to
 *          exactly one or two of its horizontal and vertical neighbors.
 * https://oeis.org/A185818
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) + 15*a(n-2) - 32*a(n-4) - 64*a(n-5), with
 *            a(1)=1, a(2)=9, a(3)=76, a(4)=656, a(5)=5680
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* static cache for memoisation (index 0 unused) */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* highest index that contains a value */
static size_t cache_cap = 0;   /* current allocation size */

/* grow the cache to at least 'needed' slots (including index 0) */
static void ensure_cache_capacity(size_t needed)
{
    if (cache_cap < needed) {
        size_t newcap = cache_cap ? cache_cap * 2 : 8;
        while (newcap < needed)
            newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (tmp == NULL)
            abort();                /* allocation failure */
        cache = tmp;
        cache_cap = newcap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)   /* not used by the sequence; return 0 as guard */
        return 0;

    /* convert to size_t for indexing */
    size_t up = (size_t)n;
    ensure_cache_capacity(up + 1);    /* indices 0..up inclusive */

    if (up > cache_len) {
        /* compute missing terms */
        size_t start = (cache_len >= 1) ? cache_len + 1 : 1;
        for (size_t i = start; i <= up; ++i) {
            seq_t val;
            switch (i) {
                case 1: val = 1; break;
                case 2: val = 9; break;
                case 3: val = 76; break;
                case 4: val = 656; break;
                case 5: val = 5680; break;
                default:
                    val = cache[i - 1] * 7
                        + cache[i - 2] * 15
                        - cache[i - 4] * 32
                        - cache[i - 5] * 64;
            }
            cache[i] = val;
        }
        cache_len = up;
    }

    return cache[up];
}
