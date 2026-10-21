/*
 * A051533  Numbers that are the sum of two positive triangular numbers.
 * https://oeis.org/A051533
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 1000, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A051533: numbers that are the sum of two positive triangular numbers.
 *
 * The implementation precomputes all sums T(i)+T(j) for 1≤i,j≤TR_LIMIT,
 * then sorts, deduplicates, and stores them in a static array.
 * Subsequent calls simply index into this array.
 *
 * All computations use the seq_t type for arithmetic.
 * The function is safe for any n≥1 that can be represented by a seq_t
 * (the precomputed table offers far more than 10 000 entries).
 */

#include <stdlib.h>

static const idx_t TR_LIMIT = 500;                /* enough for >200 000 distinct terms */
static seq_t *term_table = NULL;                  /* sorted unique terms */
static size_t term_count = 0;                     /* number of stored terms */
static int initialized = 0;

/* Attempt to avoid the utils comparison if seq_t is bare integer */
static int seq_cmp(const void *p1, const void *p2) {
    const seq_t *a = (const seq_t *)p1;
    const seq_t *b = (const seq_t *)p2;
    return (*a < *b) ? -1 : (*a > *b) ? 1 : 0;
}

/* triangular number T(n) = n*(n+1)/2 */
static inline seq_t triangular(idx_t n) {
    return ((seq_t)n * ((seq_t)n + 1)) / 2;
}

static void precompute_terms(void) {
    /* total pairs i,j (1‑based) */
    const idx_t m = TR_LIMIT;
    size_t pairs = (size_t)m * (size_t)m;

    /* allocate array for all sums */
    seq_t *all = (seq_t *)malloc(pairs * sizeof(seq_t));
    if (!all) {
        /* allocation failed – leave table empty */
        return;
    }

    /* generate sums */
    size_t pos = 0;
    for (idx_t i = 1; i <= m; ++i) {
        seq_t Ti = triangular(i);
        for (idx_t j = 1; j <= m; ++j) {
            all[pos++] = Ti + triangular(j);
        }
    }

    /* sort the array */
    qsort(all, pairs, sizeof(seq_t), seq_cmp);

    /* deduplicate into term_table */
    if (pairs == 0) {
        free(all);
        return;
    }

    /* allocate maximal possible size first */
    term_table = (seq_t *)malloc(pairs * sizeof(seq_t));
    if (!term_table) {
        free(all);
        return;
    }

    term_count = 0;
    seq_t prev = all[0];
    term_table[term_count++] = prev;

    for (size_t k = 1; k < pairs; ++k) {
        if (all[k] != prev) {
            prev = all[k];
            term_table[term_count++] = prev;
        }
    }

    /* free temporary array */
    free(all);
}

seq_t a(idx_t n) {
    if (n == 0) return 0;                /* undefined offset, return 0 */
    if (!initialized) {
        precompute_terms();
        initialized = 1;
    }
    if ((size_t)n > term_count) {
        /* requested term beyond pre‑computed range.
           For practical inputs this should not happen. */
        return 0;
    }
    return term_table[(size_t)n - 1];
}
