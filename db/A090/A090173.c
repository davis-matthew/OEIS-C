/*
 * A090173  Triangle read by rows, related to Pascal's triangle read mod 2,
 *          starting with 0, 0, 1.
 * https://oeis.org/A090173
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A090173
   Triangle read by rows, related to Pascal's triangle read mod 2.
   Recurrence (mod 2):
       T(n,k) = T(n-1,k) + T(n-1,k-1) + T(n-2,k) + T(n-2,k-1) + T(n-2,k-2),
                 n >= 2, 0 <= k.
   Initial rows:  T(0,0)=0,  T(1,0)=0, T(1,1)=1.
   The sequence a(n) is obtained by reading the triangle left‑to‑right,
   top‑to‑bottom (a(0)=T(0,0), a(1)=T(1,0), a(2)=T(1,1), ...).
   The function below builds the rows incrementally, caching all computed
   values so that invocations in arbitrary order are supported. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Forward declarations of static helpers */
static void init_static(void);
static void ensure_computed(idx_t n);

/* Row storage: rows[i] points to the i‑th row (length i+1) */
static seq_t **rows      = NULL;
static idx_t rows_cap    = 0;   /* capacity of the row pointer array */

/* Cached flattened sequence values */
static seq_t *cache      = NULL;
static size_t cache_cap  = 0;   /* capacity of the cache array */
static size_t cache_size = 0;   /* number of values stored */

/* The last fully computed row index (0‑based) */
static idx_t last_row = 0;

/* Grow the rows array if needed */
static void grow_rows_if_needed(idx_t needed) {
    if (needed <= rows_cap) return;
    size_t new_cap = rows_cap ? rows_cap * 2 : 8;
    seq_t **new_rows = (seq_t**)realloc(rows, new_cap * sizeof(seq_t*));
    rows = new_rows;
    rows_cap = new_cap;
}

/* Grow the cache array if needed */
static void grow_cache_if_needed(size_t needed) {
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 32;
    seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    cache = new_cache;
    cache_cap = new_cap;
}

/* Compute the row with index r (0‑based).  r >= 2 is guaranteed. */
static void compute_row(idx_t r) {
    grow_rows_if_needed(r + 1);
    size_t len = (size_t)(r + 1);                 /* row length */
    seq_t *prev  = rows[r - 1];                   /* length r */
    seq_t *prev2 = rows[r - 2];                   /* length r-1 */
    seq_t *cur   = (seq_t*)malloc(len * sizeof(seq_t));

    for (size_t k = 0; k < len; ++k) {
        seq_t a = (k <= r-1)          ? prev[k]   : 0;
        seq_t b = (k > 0)             ? prev[k-1] : 0;
        seq_t c = (k <= r-2)          ? prev2[k]  : 0;
        /* d = T(n-2, k-1); valid only if k-1 <= r-2, i.e. k <= r-1 */
        seq_t d = (k > 0 && k <= r-1) ? prev2[k-1] : 0;
        seq_t e = (k > 1) ? prev2[k-2] : 0;  /* always safe because k-2 <= r-2 for k>=2 */

        cur[k] = (a ^ b ^ c ^ d ^ e) & 1;

        grow_cache_if_needed(cache_size + 1);
        cache[cache_size++] = cur[k];
    }
    rows[r] = cur;
    if (r > last_row) last_row = r;
}

/* Initialise the data structures with the first two rows */
static void init_static(void) {
    if (rows != NULL) return;   /* already initialised */
    /* Row array */
    rows_cap = 8;
    rows = (seq_t**)malloc(rows_cap * sizeof(seq_t*));

    /* row 0 : [0] */
    seq_t *row0 = (seq_t*)malloc(sizeof(seq_t));
    row0[0] = 0;
    rows[0] = row0;

    /* row 1 : [0,1] */
    seq_t *row1 = (seq_t*)malloc(2 * sizeof(seq_t));
    row1[0] = 0;
    row1[1] = 1;
    rows[1] = row1;

    /* Cached values for indices 0,1,2 */
    cache_cap = 32;
    cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
    cache_size = 0;
    cache[cache_size++] = row0[0];      /* a(0) */
    cache[cache_size++] = row1[0];      /* a(1) */
    cache[cache_size++] = row1[1];      /* a(2) */

    last_row = 1;
}

/* Compute all rows up to the one that contains the requested index n */
static void ensure_computed(idx_t n) {
    init_static();
    if (n < cache_size) return;   /* already computed */

    idx_t r = last_row + 1;
    while (true) {
        /* Starting index of row r */
        idx_t start = (idx_t)(r * (r + 1) / 2);
        if (start > n) break;      /* the required index is in the previous rows */
        compute_row(r);
        ++r;
    }
}

/* Public function required by the build */
seq_t a(idx_t n) {
    ensure_computed(n);
    return cache[n];
}
