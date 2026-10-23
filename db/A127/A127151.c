/*
 * A127151  Triangle read by rows: T(n,k) is the number of full binary trees
 *          with n internal vertices and Strahler number k.
 * https://oeis.org/A127151
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = number of full binary trees with n‑1 internal vertices
 *            read row‑major from the triangle T(m,k) that counts trees with m
 *            internal vertices and Strahler number k+1. The implementation
 *            builds the triangle incrementally and answers any query
 * Range:     u8 n <= 11, u16 n <= 26, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/floor_log2_uint64.h"

static seq_t **T = NULL;          /* T[row] → array of values (index 0 unused for row>0) */
static idx_t *rowLengths = NULL;  /* number of columns stored for each row (floor(log2(row+1))) */
static idx_t *pref = NULL;        /* cumulative number of terms up to and including this row */
static size_t cap = 0;            /* allocated capacity for the arrays above */
static size_t used = 0;           /* number of rows currently stored (including row 0) */

/* ensure that capacity for at least n rows is available */
static void ensure_capacity(size_t n) {
    if (cap < n) {
        size_t new_cap = (cap > 0) ? cap * 2 : 8;
        if (new_cap < n) new_cap = n * 2;
        T = (seq_t **)realloc(T, new_cap * sizeof(seq_t *));
        rowLengths = (idx_t *)realloc(rowLengths, new_cap * sizeof(idx_t));
        pref = (idx_t *)realloc(pref, new_cap * sizeof(idx_t));
        cap = new_cap;
    }
}

/* add the next row of the triangle (row index = used) */
static void add_row(void) {
    idx_t n = (idx_t)used;                 /* new row number (0‑based) */
    idx_t len = floor_log2_uint64(n + 1);   /* number of columns for this row (0 for n==0) */

    /* allocate space for the new row */
    seq_t *row = (seq_t *)calloc((size_t)len + 1, sizeof(seq_t));
    if (row == NULL) {
        /* allocation failure – in the OEIS test environment this should not happen */
        exit(EXIT_FAILURE);
    }

    if (n == 0) {
        /* leaf row */
        row[0] = (seq_t)1;
    } else {
        /* compute recurrence */
        for (idx_t i = 0; i < n; ++i) {
            idx_t j = n - 1 - i;
            const seq_t *Ti = T[i];
            const seq_t *Tj = T[j];
            idx_t len_i = rowLengths[i];
            idx_t len_j = rowLengths[j];
            for (idx_t k1 = 0; k1 <= len_i; ++k1) {
                seq_t val1 = Ti[k1];
                if (val1 == (seq_t)0) continue;
                for (idx_t k2 = 0; k2 <= len_j; ++k2) {
                    seq_t val2 = Tj[k2];
                    if (val2 == (seq_t)0) continue;
                    idx_t rootK = (k1 == k2) ? (k1 + 1) : ((k1 > k2) ? k1 : k2);
                    if (rootK <= len) {
                        row[rootK] += val1 * val2;
                    }
                }
            }
        }
    }

    /* store the new row */
    T[used] = row;
    rowLengths[used] = len;
    pref[used] = pref[used - 1] + len;
    ++used;
}

/* expand the table until the global index target is covered */
static void ensure_up_to(idx_t target) {
    if (used == 0) {
        /* first time: allocate initial capacity and add row 0 */
        ensure_capacity(16);
        pref = (idx_t *)calloc(16, sizeof(idx_t));
        if (pref == NULL) exit(EXIT_FAILURE);
        pref[0] = 0;
        rowLengths = (idx_t *)calloc(16, sizeof(idx_t));
        if (rowLengths == NULL) exit(EXIT_FAILURE);
        rowLengths[0] = 0;
        T = (seq_t **)calloc(16, sizeof(seq_t *));
        if (T == NULL) exit(EXIT_FAILURE);
        T[0] = (seq_t *)calloc(1, sizeof(seq_t));
        if (T[0] == NULL) exit(EXIT_FAILURE);
        T[0][0] = (seq_t)1; /* leaf */
        used = 1;
    }
    while (pref[used - 1] < target) {
        ensure_capacity(used + 8);
        add_row();
    }
}

seq_t a(idx_t n) {
    /* n is 1‑based sequence index */
    if (n < 1) return (seq_t)0; /* undefined, but guard against overflows */

    ensure_up_to(n);

    /* locate the row containing this index */
    size_t lo = 1, hi = used;
    while (lo < hi) {
        size_t mid = (lo + hi) / 2;
        if (pref[mid] >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t row = (idx_t)lo;
    size_t col = (size_t)(n - pref[row - 1]); /* 1‑based column index */

    return T[row][col];
}
