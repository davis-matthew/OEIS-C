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
 * Validated: partial, pass, 77 terms through n = 76 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ------------------------------------------ */
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../utils/mem.h"

/* ----------------------------------------------------------- */
static idx_t floor_triangle(idx_t n)
/* largest d such that d*(d+1)/2 ≤ n */
{
    /* initial estimate using floating point */
    long double d_est = (sqrtl((long double)8.0L * (long double)n + 1.0L) - 1.0L) / 2.0L;
    idx_t d = (idx_t)d_est;

    /* correct upward */
    while (((unsigned __int128)(d + 1) * (d + 2) / 2) <= n) ++d;
    /* correct downward */
    while (((unsigned __int128)d * (d + 1) / 2) > n) --d;
    return d;
}

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
            seq_t *ptr = mem_resize(seq_t, rows[i], step_needed);
            if (!ptr) continue;                   /* ignore if realloc fails */
            /* zero new tail elements */
            mem_set(ptr + step, 0, (step_needed - step) * sizeof(seq_t));
            rows[i] = ptr;
        }

        /* allocate new rows */
        for (size_t i = step; i < step_needed; ++i) {
            rows[i] = mem_new(seq_t, step_needed);
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
        seq_t *new_cache = mem_resize(seq_t, cache, cache_needed);
        if (!new_cache) return;                   /* ignore failure */
        /* zero new entries */
        mem_set(new_cache + cache_size, 0, (cache_needed - cache_size) * sizeof(seq_t));
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
