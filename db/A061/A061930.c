/*
 * A061930  Square array read by antidiagonals of
 *          T(n,k)=T(n-1,[k/2])+T(n-1,[k/3]) with T(0,0)=1.
 * https://oeis.org/A061930
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is obtained by reading the triangle T(n,k) defined by
 *            T(0,0)=1 and T(n,k)=T(n-1,⌊k/2⌋)+T(n-1,⌊k/3⌋) for n>0, k≥0 along
 *            antidiagonals. a(0)=T(0,0), a(1)=T(0,1), a(2)=T(1,0),
 *            a(3)=T(0,2), …
 * Range:     u8 n <= 43, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   floor_triangle
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ------------------------------------------ */
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../utils/floor_triangle.h"

/* ----------------------------------------------------------- */
/* forward declarations of helpers                         */
static idx_t floor_triangle(idx_t n);
static void extend_to(idx_t n);

/* ----------------------------------------------------------- */
/* dynamic storage                                         */
static seq_t **rows = NULL;          /* rows of the triangle */
static size_t step = 0;              /* current triangle height = columns */

static seq_t *cache = NULL;          /* cache of a(n) */
static size_t cache_size = 0;        /* size of the cache */

/* ----------------------------------------------------------- */
static void extend_to(idx_t n)
/* Build the triangle up to the antidiagonal that contains n
   and fill cache entries up to n.                           */
{
    if (n < (idx_t)cache_size) return;            /* already covered */

    idx_t d_needed = floor_triangle(n);
    size_t step_needed = d_needed + 1;            /* rows and columns */

    /* ----- grow triangle ---------------------------------- */
    if (step_needed > step) {
        /* enlarge the array of row pointers */
        seq_t **new_rows = (seq_t**)realloc(rows, step_needed * sizeof(seq_t*));
        if (!new_rows) return;                    /* allocation failure, ignore */
        rows = new_rows;

        /* enlarge existing rows to new column width */
        for (size_t i = 0; i < step; ++i) {
            seq_t *ptr = (seq_t*)realloc(rows[i], step_needed * sizeof(seq_t));
            if (!ptr) continue;                   /* ignore if realloc fails */
            /* zero new tail elements */
            memset(ptr + step, 0, (step_needed - step) * sizeof(seq_t));
            rows[i] = ptr;
        }

        /* allocate new rows */
        for (size_t i = step; i < step_needed; ++i) {
            rows[i] = (seq_t*)calloc(step_needed, sizeof(seq_t));
            if (!rows[i]) continue;               /* ignore if calloc fails */
        }

        step = step_needed;
    }

    /* ----- compute triangle values -------------------------- */
    /* row 0 is already zero except T(0,0)=1 */
    rows[0][0] = (seq_t)1;

    for (size_t i = 1; i < step; ++i) {
        for (size_t j = 0; j < step; ++j) {
            rows[i][j] = rows[i-1][j/2] + rows[i-1][j/3];
        }
    }

    /* ----- grow cache -------------------------------------- */
    size_t old_cache_size = cache_size;
    size_t cache_needed = (size_t)n + 1;
    if (cache_needed > cache_size) {
        seq_t *new_cache = (seq_t*)realloc(cache, cache_needed * sizeof(seq_t));
        if (!new_cache) return;                   /* ignore failure */
        /* zero new entries */
        memset(new_cache + cache_size, 0,
               (cache_needed - cache_size) * sizeof(seq_t));
        cache = new_cache;
        cache_size = cache_needed;
    }

    /* ----- fill new cache entries --------------------------- */
    for (size_t i = 0; i < step; ++i) {
        for (size_t j = 0; j < step; ++j) {
            idx_t d = (idx_t)i + (idx_t)j;
            idx_t tri = (idx_t)((unsigned __int128)d * (d + 1) / 2);
            idx_t idx = tri + (idx_t)i;           /* sequence index */

            if (idx >= old_cache_size && idx <= n) {
                cache[idx] = rows[i][j];
            }
        }
    }
}

/* ----------------------------------------------------------- */
seq_t a(idx_t n)
/* Return a(n) – the OEIS sequence A061930 value at index n */
{
    if (n >= (idx_t)cache_size) extend_to(n);
    return cache[(size_t)n];
}
