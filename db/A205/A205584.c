/*
 * A205584  Number of (n+1) X 3 0..2 arrays with no 2 X 2 subblock having the
 *          same number of equal edges as its horizontal or vertical
 *          neighbors, and new values 0..2 introduced in row major order.
 * https://oeis.org/A205584
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) +24*a(n-2) +82*a(n-3) +83*a(n-4) -107*a(n-5)
 *            +30*a(n-6) +250*a(n-7); Initial values: a(1)=74, a(2)=550,
 *            a(3)=4220, a(4)=32500, a(5)=248940, a(6)=1910922, a(7)=14668206
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* static cache for memoisation */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of terms currently computed */
static size_t cache_cap = 0;   /* allocated size of cache */

/* allocate or reallocate the cache array, keeping existing entries */
static void ensure_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap : 8;
    while (new_cap < needed)
        new_cap <<= 1;

    seq_t *new_arr;
#ifdef __cplusplus
    new_arr = new seq_t[new_cap];
#else
    new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
#endif

    for (size_t i = 0; i < cache_len; ++i)
        new_arr[i] = cache[i];

#ifdef __cplusplus
    delete[] cache;
#else
    free(cache);
#endif

    cache       = new_arr;
    cache_cap   = new_cap;
}

/* Answer for given index n (n >= 1) */
seq_t a(idx_t n)
{
    if (n == 0)          /* undefined below offset */
        return 0;

    size_t needed = (size_t)n;
    ensure_capacity(needed);

    static const seq_t init[7] = {
        74ULL, 550ULL, 4220ULL, 32500ULL,
        248940ULL, 1910922ULL, 14668206ULL
    };

    /* Fill missing entries */
    for (size_t pos = cache_len; pos < needed; ++pos) {
        if (pos < 7) {                     /* initial values */
            cache[pos] = init[pos];
        } else {                           /* recurrence */
            seq_t val = cache[pos - 1] * 3ULL;
            val += cache[pos - 2] * 24ULL;
            val += cache[pos - 3] * 82ULL;
            val += cache[pos - 4] * 83ULL;
            val -= cache[pos - 5] * 107ULL;
            val += cache[pos - 6] * 30ULL;
            val += cache[pos - 7] * 250ULL;
            cache[pos] = val;
        }
    }

    cache_len = needed;
    return cache[needed - 1];
}
