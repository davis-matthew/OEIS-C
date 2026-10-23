/*
 * A034036  Numbers of the form (x^2+xy+2y^2 with x>=0, y>=0).
 * https://oeis.org/A034036
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth (1‑based) non‑negative integer that can be
 *            written as x*x + x*y + 2*y*y with x>=0 and y>=0, sorted in
 *            increasing order
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
/* idx_t and seq_t are supplied by the build; they are unsigned integer types. */

static idx_t *cache = NULL;   /* array of the generated values (idx_t type) */
static size_t cache_len = 0;  /* number of elements in cache  */
static idx_t cur_limit = 0;   /* y,x limit used for the last generation */

/* Comparator for qsort on idx_t values */
static int cmp_u64(const void *a, const void *b)
{
    idx_t va = *(const idx_t *)a;
    idx_t vb = *(const idx_t *)b;
    return (va > vb) - (va < vb);
}

/* Generate all values x^2 + x*y + 2*y^2 for 0<=x,y<=limit.
   The results are sorted, deduplicated, and stored in the global cache. */
static void generate_cache(idx_t new_limit)
{
    /* Estimate the number of pairs; add 1 to account for 0. */
    size_t total = (size_t)(new_limit + 1) * (new_limit + 1);
    idx_t *tmp = (idx_t *)malloc(total * sizeof(idx_t));
    if (!tmp) return; /* in case of OOM – the caller will crash later */

    size_t len = 0;
    for (idx_t y = 0; y <= new_limit; ++y) {
        idx_t y2 = y * y;
        for (idx_t x = 0; x <= new_limit; ++x) {
            idx_t val = x * x + x * y + (idx_t)2 * y2;
            tmp[len++] = val;
        }
    }

    /* Sort all produced numbers */
    qsort(tmp, len, sizeof(idx_t), cmp_u64);

    /* Deduplicate in place */
    size_t uniq = 0;
    for (size_t i = 0; i < len; ++i) {
        if (uniq == 0 || tmp[i] != tmp[uniq - 1]) {
            tmp[uniq++] = tmp[i];
        }
    }

    free(cache);
    cache     = tmp;
    cache_len = uniq;
    cur_limit = new_limit;
}

/* Ensure that the cache contains at least n elements.
   The cache is enlarged by doubling the limit until sufficient. */
static void ensure_cache(idx_t n)
{
    if (cache_len >= n) return;

    idx_t new_lim = cur_limit ? cur_limit * 2 : 256; /* start with a small table */
    while (1) {
        generate_cache(new_lim);
        if (cache_len >= n) break;
        new_lim *= 2;
    }
}

/* Public sequence function.  seq_t is cast from the stored idx_t value. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* index 0 is never asked, but defined as 0 */
    ensure_cache(n);
    return (seq_t)cache[n - 1];
}
