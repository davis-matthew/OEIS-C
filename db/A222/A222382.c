/*
 * A222382  Sum of neighbor maps: number of n X 3 binary arrays indicating the
 *          locations of corresponding elements equal to the sum mod 3 of
 *          their horizontal and antidiagonal neighbors in a random 0..2 n X 3
 *          array.
 * https://oeis.org/A222382
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-1) + 8*a(n-2) - 64*a(n-3) - 16*a(n-4) + 128*a(n-5)
 *            (n > 5) with initial values a(1) = 8 a(2) = 48 a(3) = 512 a(4) =
 *            3968 a(5) = 32768
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          // array of computed terms
static idx_t cache_len = 0;          // number of terms stored (a(1) is at cache[0])
static size_t cache_alloc = 0;       // allocated capacity of cache

seq_t a(idx_t n) {
    /* ensure cache has at least the first five terms */
    static const seq_t init[5] = {8, 48, 512, 3968, 32768};

    /* allocate initial capacity if needed */
    if (cache == NULL) {
        cache_alloc = 16;                           /* start with a small block */
        cache = (seq_t *)malloc(cache_alloc * sizeof(seq_t));
        if (cache == NULL) return 0;                /* allocation failed */
    }

    /* fill initial terms up to min(5, n) */
    if (cache_len < 5) {
        idx_t last = (cache_len < 5) ? 5 : cache_len;
        if (last > cache_len) {
            if (last > cache_alloc) {
                size_t new_alloc = cache_alloc;
                while (new_alloc < (size_t)last)
                    new_alloc *= 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_alloc * sizeof(seq_t));
                if (tmp == NULL) return 0;
                cache = tmp;
                cache_alloc = new_alloc;
            }
            for (idx_t i = cache_len + 1; i <= last; ++i)
                cache[i - 1] = init[i - 1];
            cache_len = last;
        }
    }

    /* compute terms up to n (if needed) */
    if (n > cache_len) {
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            /* ensure capacity for new term */
            if ((size_t)i > cache_alloc) {
                size_t new_alloc = cache_alloc ? cache_alloc * 2 : 16;
                while (new_alloc < (size_t)i)
                    new_alloc *= 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_alloc * sizeof(seq_t));
                if (tmp == NULL) return 0;
                cache = tmp;
                cache_alloc = new_alloc;
            }

            /* recurrence: a(i) = 8*a(i-1) + 8*a(i-2) - 64*a(i-3) - 16*a(i-4) + 128*a(i-5) */
            seq_t val = 8 * cache[i - 2]          /* a(i-1) */
                      + 8 * cache[i - 3]          /* a(i-2) */
                      - 64 * cache[i - 4]         /* a(i-3) */
                      - 16 * cache[i - 5]         /* a(i-4) */
                      + 128 * cache[i - 6];       /* a(i-5) */
            cache[i - 1] = val;
        }
        cache_len = n;
    }

    return cache[n - 1];
}
