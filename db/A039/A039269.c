/*
 * A039269  Numbers whose base-12 representation has the same number of 8's
 *          and 9's.
 * https://oeis.org/A039269
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   Numbers whose base-12 representation has the same number of 8's
 *            and 9's
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Return 1 iff x's base‑12 representation contains an equal number of 8's and 9's. */
static int balanced_in_base12(uint64_t x)
{
    uint64_t c8 = 0, c9 = 0;
    while (x) {
        uint64_t d = x % 12;
        if (d == 8) ++c8;
        else if (d == 9) ++c9;
        x /= 12;
    }
    return c8 == c9;
}

/* Cached sequence */
static seq_t *cache       = NULL;   /* pointer to stored terms */
static size_t cache_cap   = 0;      /* allocated capacity in terms of elements */
static size_t cache_len   = 0;      /* number of terms actually stored */
static uint64_t last_checked = 0;   /* last integer examined during the search */

/* Grow the cache array to accommodate at least 'needed' terms. */
static void grow_cache(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = needed + 10; /* a small buffer to reduce realloc calls */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();            /* out of memory – terminate */
    cache = tmp;
    cache_cap = new_cap;
}

/* Ensure that the cache contains at least 'n' terms. */
static void ensure_cache(idx_t n)
{
    while (cache_len < n) {
        if (balanced_in_base12(last_checked)) {
            grow_cache(cache_len + 1);
            cache[cache_len++] = (seq_t)last_checked;
        }
        ++last_checked;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;       /* safety for offset 1 */
    ensure_cache(n);
    return cache[n - 1];
}
