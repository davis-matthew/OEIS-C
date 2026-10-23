/*
 * A144677  Related to enumeration of quantum states (see reference for
 *          precise definition).
 * https://oeis.org/A144677
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=2, a(2)=3, a(3)=6, a(4)=9, a(5)=12, a(6)=18,
 *            a(7)=24
 *            a(n)=2*a(n-1)-a(n-2)+2*a(n-3)-4*a(n-4)+2*a(n-5)-a(n-6)+2*a(n-7)-a(n-8)
 *            for all n ≥ 8 (OEIS A144677)
 * Range:     u8 n <= 20, u16 n <= 148, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;        /* array of computed terms */
static size_t cache_len = 0;       /* number of terms actually stored   */
static size_t cache_cap = 0;       /* allocated capacity                 */

seq_t a(idx_t n)
{
    /* Initialise cache on first call */
    if (cache_cap == 0) {
        cache_cap = 8;
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        if (!cache) {              /* allocation failure – return 0 */
            return (seq_t)0;
        }
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)6;
        cache[4] = (seq_t)9;
        cache[5] = (seq_t)12;
        cache[6] = (seq_t)18;
        cache[7] = (seq_t)24;
        cache_len = 8;
    }

    if ((size_t)n < cache_len)
        return cache[(size_t)n];

    /* Expand capacity if necessary */
    size_t old_len = cache_len;
    if (n + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return cache[cache_len - 1]; /* keep old cache on failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms using the linear recurrence */
    for (size_t i = old_len; i <= (size_t)n; ++i) {
        if (i < 8) {
            /* already initialized */
            continue;
        }
        /* recurrence:   2*a(i-1) - a(i-2) + 2*a(i-3) - 4*a(i-4)
                         + 2*a(i-5) - a(i-6) + 2*a(i-7) - a(i-8) */
        cache[i] =
            (((cache[i-1] << 1) - cache[i-2]) +
             ((cache[i-3] << 1) - (cache[i-4] << 2)) +
             ((cache[i-5] << 1) - cache[i-6]) +
             ((cache[i-7] << 1) - cache[i-8]));
    }
    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
