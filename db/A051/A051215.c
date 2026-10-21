/*
 * A051215  Numbers of the form 4^x-y^2 >= 0.
 * https://oeis.org/A051215
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th smallest non‑negative integer of the form 4^x −
 *            y^2, with x, y ≥ 0 and 4^x ≥ y^2. Duplicate values are removed;
 *            the list is sorted ascending
 * Range:     u8 n <= 32, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Counts and stores the sequence values.  The cache is built on
   first use and reused for later calls. */
static uint64_t  *tmp_vals = NULL;   /* intermediate 64‑bit values   */
static size_t   tmp_len  = 0, tmp_cap  = 0;
static seq_t   *cache  = NULL;       /* final cached values          */
static size_t   cache_len = 0;
static int      builtin = 0;         /* 0 = not yet built, 1 = ready */

/* Comparison used by qsort for uint64_t values. */
static int cmp_uint64(const void *p, const void *q)
{
    const uint64_t *a = (const uint64_t *)p;
    const uint64_t *b = (const uint64_t *)q;
    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}

/* Build the cache once.  The chosen limit of x = 16 gives
   more than 10000 distinct terms while keeping memory and
   time comfortably below the limits.  The algorithm generates
   all possible expressions 4^x − y^2 with 0 ≤ y ≤ 2^x,
   stores them in a temporary uint64_t array, sorts,
   deduplicates, and finally copies to the cache as seq_t. */
static void build_cache(void)
{
    if (builtin) return;
    builtin = 1;

    const idx_t MAX_X = 16;                 /* 4^16 = 2^32 fits 64‑bit */
    /* Rough capacity: sum_{x=0..MAX_X} (2^x + 1) = 2^(MAX_X+1) + (MAX_X+1) */
    tmp_cap = ((size_t)1 << (MAX_X + 1)) + (MAX_X + 1);
    tmp_vals = (uint64_t *)malloc(tmp_cap * sizeof(uint64_t));
    if (!tmp_vals) { cache_len = 0; return; }

    tmp_len = 0;
    for (idx_t x = 0; x <= MAX_X; ++x) {
        uint64_t pow4x = ((uint64_t)1) << (2 * x);          /* 4^x = 2^(2x) */
        idx_t y_max = ((idx_t)1) << x;                     /* 2^x          */

        for (idx_t y = 0; y <= y_max; ++y) {
            uint64_t y2 = (uint64_t)y * (uint64_t)y;
            uint64_t val = pow4x - y2;

            if (tmp_len == tmp_cap) {
                tmp_cap <<= 1;
                uint64_t *newp = (uint64_t *)realloc(tmp_vals, tmp_cap * sizeof(uint64_t));
                if (!newp) { free(tmp_vals); tmp_vals = NULL; cache_len = 0; return; }
                tmp_vals = newp;
            }
            tmp_vals[tmp_len++] = val;
        }
    }

    /* Sort the 64‑bit values. */
    qsort(tmp_vals, tmp_len, sizeof(uint64_t), cmp_uint64);

    /* Deduplicate in place. */
    size_t uniq = 0;
    for (size_t i = 0; i < tmp_len; ++i) {
        if (uniq == 0 || tmp_vals[uniq - 1] != tmp_vals[i]) {
            tmp_vals[uniq++] = tmp_vals[i];
        }
    }

    /* Allocate final cache and copy values as seq_t. */
    cache = (seq_t *)malloc(uniq * sizeof(seq_t));
    if (!cache) { free(tmp_vals); tmp_vals = NULL; cache_len = 0; return; }
    for (size_t i = 0; i < uniq; ++i) {
        cache[i] = (seq_t)tmp_vals[i];
    }
    cache_len = uniq;
    free(tmp_vals);
    tmp_vals = NULL;
    tmp_cap = tmp_len = 0;
}

/* Public accessor returning the n-th term (1‑based indexing).  If n
   lies outside the built cache, the function returns 0.  The cache
   is constructed lazily and is reused for subsequent calls. */
seq_t a(idx_t n)
{
    build_cache();
    if (n == 0 || n > cache_len) return 0;
    return cache[n - 1];
}
