/*
 * A085780  Numbers that are a product of 2 triangular numbers.
 * https://oeis.org/A085780
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   the n‑th term is the n‑th distinct number of the form T(i)*T(j)
 *            where T(k)=k*(k+1)/2 for k≥0, sorted increasingly. The function
 *            builds a static cache of the first ≈10 000 such numbers on its
 *            first call and returns the requested term
 * Range:     u8 n <= 1, u16 n <= 16, u32 n <= 7381, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Compare two seq_t values for qsort or min selections.  */
#if defined(__cplusplus)
  #include <algorithm>
  static inline int cmp_seq_t(const void *a, const void *b) {
      const seq_t *x = (const seq_t *)a, *y = (const seq_t *)b;
      if (*x < *y) return -1;
      if (*x > *y) return 1;
      return 0;
  }
#else
  static inline int cmp_seq_t(const void *a, const void *b) {
      const seq_t *x = (const seq_t *)a, *y = (const seq_t *)b;
      return (*x < *y) ? -1 : (*x > *y) ? 1 : 0;
  }
#endif

/* ----- static data for the memoised cache ----- */
static seq_t *terms      = NULL;   /* sorted distinct products            */
static idx_t  terms_len  = 0;      /* number of cached terms wanted       */
static idx_t  terms_cap  = 0;      /* allocated capacity of terms         */

static seq_t *tri        = NULL;   /* triangular numbers T(k)             */
static idx_t  tri_len    = 0;      /* number of triangular numbers stored */
static idx_t  MAX_I      = 2100;   /* number of rows to consider          */

static idx_t *col        = NULL;   /* current column for each row i       */
static idx_t  col_len    = 0;      /* allocated length of col array       */

/* ----- helper: extend triangular numbers to index >= n ----- */
static void ensure_tri(idx_t n) {
    if (tri_len > n) return;
    size_t new_len = n + 1;
    seq_t *tmp = (seq_t *)realloc(tri, new_len * sizeof(seq_t));
    if (!tmp) return;   /* allocation failed – we abort gracefully */
    tri = tmp;
    /* fill new entries */
    for (idx_t k = tri_len; k <= n; ++k) {
        seq_t v = ((seq_t)k) * ((seq_t)(k + 1)) / 2;
        tri[k] = v;
    }
    tri_len = new_len;
}

/* ----- helper: extend col array to store at least up to n rows ----- */
static void ensure_col(idx_t n) {
    if (col_len > n) return;
    size_t new_len = n + 1;
    idx_t *tmp = (idx_t *)realloc(col, new_len * sizeof(idx_t));
    if (!tmp) return;
    col = tmp;
    for (idx_t i = col_len; i <= n; ++i) col[i] = 0;
    col_len = new_len;
}

/* ----- main function ----- */
seq_t a(idx_t n) {
    if (n == 0) return 0;   /* index starts at 1 */

    /* initialise static data on first call */
    if (!terms) {
        /* triangular numbers from T(0) up to T(MAX_I) */
        tri = (seq_t *)malloc((MAX_I + 1) * sizeof(seq_t));
        if (!tri) return 0;
        tri[0] = 0;
        for (idx_t i = 1; i <= MAX_I; ++i)
            tri[i] = ((seq_t)i) * ((seq_t)(i + 1)) / 2;
        tri_len = MAX_I + 1;

        /* column indices for each row */
        col = (idx_t *)malloc((MAX_I + 1) * sizeof(idx_t));
        if (!col) { free(tri); return 0; }
        for (idx_t i = 0; i <= MAX_I; ++i) col[i] = i;
        col_len = MAX_I + 1;

        /* terms cache: start with term 0 */
        terms_cap = 20000;          /* initial capacity > target 10k */
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
        if (!terms) { free(tri); free(col); return 0; }
        terms[0] = 0;
        terms_len = 1;
    }

    /* generate more terms until we have at least n entries */
    while (terms_len < n) {
        /* find the minimal product among all rows 1..MAX_I */
        seq_t min_val = 0;
        idx_t min_row = 1;
        int first = 1;
        for (idx_t i = 1; i <= MAX_I; ++i) {
            /* ensure triangular numbers are available for current column */
            if (col[i] >= tri_len) ensure_tri(col[i]);

            seq_t prod = tri[i] * tri[col[i]];
            if (first || prod < min_val) {
                min_val = prod;
                min_row = i;
                first = 0;
            }
        }

        /* store the value if distinct from the last one */
        if (min_val != terms[terms_len - 1]) {
            if (terms_len == terms_cap) {
                terms_cap *= 2;
                seq_t *tmp = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
                if (!tmp) return 0;   /* allocation failed */
                terms = tmp;
            }
            terms[terms_len++] = min_val;
        }

        /* advance the column of the chosen row */
        col[min_row]++;
        /* ensure that the triangular array can hold the next column index */
        if (col[min_row] >= tri_len) ensure_tri(col[min_row]);
    }

    return terms[n - 1];
}
