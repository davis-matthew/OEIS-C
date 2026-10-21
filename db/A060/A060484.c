/*
 * A060484  Number of 6-block tricoverings of an n-set.
 * https://oeis.org/A060484
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A060484: number of 6‑block tricoverings of an n‑set.
   Recurrence: a(n) = 45*a(n-1) - 720*a(n-2) + 5220*a(n-3)
                - 17664*a(n-4) + 25920*a(n-5) - 12800*a(n-6)
   Initial values: a(3)=1, a(4)=95, a(5)=3107, a(6)=75835,
                   a(7)=1653771, a(8)=34384875.  For n<3 the
   function returns 0. */

#include <stddef.h>
#include <stdlib.h>

/* --- cache for previously computed terms -------------------------------- */
static seq_t *cache = NULL;    /* cache[i] = a(i) */
static idx_t  cache_len = 0;   /* number of cached entries   */
static size_t cache_cap = 0;   /* allocated capacity of cache*/

static void reserve_cache(size_t new_cap) {
    if (new_cap <= cache_cap) return;
    size_t new_size = new_cap;
    seq_t *tmp = (seq_t *)malloc(new_size * sizeof(seq_t));
    if (!tmp) abort();
    if (cache) {
        for (size_t i = 0; i < cache_len; ++i) tmp[i] = cache[i];
        free(cache);
    }
    cache = tmp;
    cache_cap = new_size;
}

static void ensure_cache(idx_t n) {
    if (n < cache_len) return;
    size_t want = (size_t)n + 1;
    reserve_cache(want);
    for (size_t i = cache_len; i < want; ++i) {
        seq_t val;
        if (i < 3) {
            val = 0;
        } else if (i == 3) {
            val = 1;
        } else if (i == 4) {
            val = 95;
        } else if (i == 5) {
            val = 3107;
        } else if (i == 6) {
            val = 75835;
        } else if (i == 7) {
            val = 1653771;
        } else if (i == 8) {
            val = 34384875;
        } else {
            /* recurrence: 45*a(n-1) - 720*a(n-2) + 5220*a(n-3)
                           - 17664*a(n-4) + 25920*a(n-5) - 12800*a(n-6) */
            val = cache[i-1] * (seq_t)45;
            val -= cache[i-2] * (seq_t)720;
            val += cache[i-3] * (seq_t)5220;
            val -= cache[i-4] * (seq_t)17664;
            val += cache[i-5] * (seq_t)25920;
            val -= cache[i-6] * (seq_t)12800;
        }
        cache[i] = val;
    }
    cache_len = want;
}

seq_t a(idx_t n) {
    if (n < 3) return 0;     /* values below the offset are undefined */
    ensure_cache(n);
    return cache[n];
}
