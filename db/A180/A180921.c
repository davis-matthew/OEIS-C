/*
 * A180921  a(n) is the square root of the sum of the cubes of the b(n)
 *          consecutive integers starting from b(n), where b(n) = A180920.
 * https://oeis.org/A180921
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 6, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A180921
 *
 * Formula:
 *     a(1) = 1
 *     a(2) = 2079
 *     a(3) = 7876385
 *     a(4) = 30254180671
 *     a(n) = 3904*a(n-1) - 238206*a(n-2) + 3904*a(n-3) - a(n-4)   (n ≥ 5)
 *
 * The function memoises terms up to the largest index requested.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* static cache for computed values; 0‑based array holding a(1) … a(cache_len) */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of terms currently stored (0 if none) */
static size_t cache_cap = 0;   /* allocated capacity, in elements */

/* extend cache to support terms up to wanted (1‑based index) */
static void ensure_cache_size(idx_t wanted)
{
    if (wanted <= cache_len)
        return;

    /* allocate first time or grow if needed */
    size_t needed = (size_t)wanted;                /* always fits 64‑bit size_t */
    if (needed > cache_cap) {
        seq_t *new_arr = (seq_t *)malloc(needed * sizeof(seq_t));
        if (!new_arr)                       /* silently drop computing */
            return;
        if (cache) {
            memcpy(new_arr, cache, cache_len * sizeof(seq_t));
            free(cache);
        }
        cache     = new_arr;
        cache_cap = needed;
    }

    /* compute missing terms, starting from current cache_len */
    for (size_t i = cache_len; i < needed; ++i) {
        if (i == 0) {                         /* a(1) */
            cache[i] = 1;
        } else if (i == 1) {                  /* a(2) */
            cache[i] = 2079;
        } else if (i == 2) {                  /* a(3) */
            cache[i] = 7876385;
        } else if (i == 3) {                  /* a(4) */
            cache[i] = 30254180671ULL;
        } else {                              /* n ≥ 5 */
            /* recurrence uses previous four terms:
             *   a(n) = 3904*a(n-1) - 238206*a(n-2) + 3904*a(n-3) - a(n-4)
             * The indices i-1,i-2,i-3,i-4 correspond to a(n-1) … a(n-4) */
            const seq_t a_nm1 = cache[i - 1];
            const seq_t a_nm2 = cache[i - 2];
            const seq_t a_nm3 = cache[i - 3];
            const seq_t a_nm4 = cache[i - 4];

            cache[i] = 3904 * a_nm1
                       - 238206 * a_nm2
                       + 3904 * a_nm3
                       - a_nm4;
        }
    }
    cache_len = needed;
}

/* required sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined for n = 0 – return 0 */

    ensure_cache_size(n);
    return cache[n - 1];           /* cache is 0‑based, n is 1‑based */
}
