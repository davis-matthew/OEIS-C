/*
 * A191852  Ordered nonnegative differences 4*f-g, where f and g are positive
 *          Fibonacci numbers (A000045).
 * https://oeis.org/A191852
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 16, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A191852: Ordered nonnegative differences 4*f - g where f,g are positive Fibonacci numbers.
    The sequence consists of all distinct non‑negative values of 4*f - g,
    where f and g run over all positive Fibonacci numbers (A000045),
    sorted increasingly.
    For example the first terms are:
       0, 1, 2, 3, 4, 5, 6, 7, 9, 10, 11, 12, 15, 17, 18, 19, 24, 27, 29, 30,...
*/

#include <stdlib.h>
#include <stddef.h>

/* Auxiliary: primary array resize routine */
static void ensure_cap(seq_t **arr, size_t *cap, size_t needed)
{
    if (*cap < needed) {
        size_t new_cap = (*cap > 0) ? (*cap << 1) : 1;
        while (new_cap < needed) new_cap <<= 1;
        *arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        *cap = new_cap;
    }
}

/* Comparator for qsort */
static int cmp_seq_t(const void *p1, const void *p2)
{
    const seq_t *a = (const seq_t *)p1;
    const seq_t *b = (const seq_t *)p2;
    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}

/* Global state: cache of the sequence */
static seq_t  *cache      = NULL;
static size_t cache_len   = 0;
static size_t cache_cap   = 0;

/* Precompute the sequence up to a fixed bound. */
static void build_sequence(void)
{
    /* Number of Fibonacci numbers to generate. 400 is enough to cover ≥80 000 distinct terms. */
    const size_t MAX_FIB = 400;

    /* Fibonacci numbers (with the duplicate 1 as in A000045). */
    static seq_t *fibs = NULL;
    static size_t fibs_len = 0, fibs_cap = 0;

    if (fibs == NULL) {
        ensure_cap(&fibs, &fibs_cap, MAX_FIB);
        fibs[0] = (seq_t)1;
        fibs_len = 1;
        if (MAX_FIB > 1) {
            fibs[1] = (seq_t)1;
            fibs_len = 2;
            for (idx_t i = 2; i < MAX_FIB; ++i) {
                fibs[i] = fibs[i - 1] + fibs[i - 2];
            }
            fibs_len = MAX_FIB;
        }
    }

    /* Temporary array for all differences before deduplication.  The maximum possible
       number of differences is MAX_FIB*(MAX_FIB+1)/2. */
    static seq_t *diffs = NULL;
    static size_t diffs_len = 0, diffs_cap = 0;
    size_t max_diffs = MAX_FIB * (MAX_FIB + 1) / 2;
    ensure_cap(&diffs, &diffs_cap, max_diffs);
    diffs_len = 0;

    for (idx_t i = 0; i < fibs_len; ++i) {
        seq_t f   = fibs[i];
        seq_t lim = f * (seq_t)4;      /* 4*f  */
        for (idx_t j = 0; j < fibs_len; ++j) {
            if (fibs[j] > lim) break;
            seq_t diff = lim - fibs[j];
            diffs[diffs_len++] = diff;
        }
    }

    /* Sort all differences */
    qsort(diffs, diffs_len, sizeof(seq_t), cmp_seq_t);

    /* Deduplicate into the global cache */
    ensure_cap(&cache, &cache_cap, diffs_len);
    if (diffs_len > 0) {
        cache[0] = diffs[0];
        cache_len = 1;
        for (size_t k = 1; k < diffs_len; ++k) {
            if (diffs[k] != diffs[k - 1]) {
                cache[cache_len++] = diffs[k];
            }
        }
    } else {
        cache_len = 0;
    }

    /* Free temporary storage (optional) */
    free(fibs);
    free(diffs);
}

/* User function ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Lazy initialization: build the first batch on the first call. */
    if (cache_len == 0) {
        build_sequence();
    }

    /* Return the requested element.  The caller guarantees n >= 1. */
    return cache[n - 1];
}
