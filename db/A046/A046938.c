/*
 * A046938  Sequence formed from rows of triangle A046937.
 * https://oeis.org/A046938
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 40, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A046938: triangle A046937 with duplicates removed
   Each row r (r ≥ 0) has length r+1 except row 0 and 1 (both length 1).
   Row 0: 1
   Row 1: 2
   For r ≥ 2:
        first element = previous row's last element + previous row's first element
        subsequent elements = cumulative sums using the previous row.
   Concatenate all rows in order, delete duplicate entries.
   The function returns the nth term (0‑based). */
#include <stdlib.h>
#include <stdint.h>

/* Helper primitives that work for both C and C++ */
#if defined(__cplusplus)
  #include <cstddef>
  static inline seq_t *seq_alloc(size_t n) { return new seq_t[n]; }
  static inline void seq_free(seq_t *p) { delete [] p; }
#else
  #include <stddef.h>
  static inline seq_t *seq_alloc(size_t n) { return (seq_t *)malloc(n * sizeof(seq_t)); }
  static inline void seq_free(seq_t *p) { free(p); }
#endif

/* Static cache for computed sequence values */
static seq_t *cache_vals = NULL;
static size_t   cache_cap = 0;
static idx_t    cache_len = 0;

/* Static storage for the most recently generated triangle row */
static seq_t *prev_row     = NULL;   /* row r-1 */
static size_t prev_len     = 0;      /* length of prev_row */
static idx_t  prev_row_idx = 0;      /* triangle index of prev_row */

/* Grow the cache array if needed */
static void cache_grow(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < needed) new_cap <<= 1;
#if defined(__cplusplus)
    seq_t *tmp = new seq_t[new_cap];
    for (size_t i = 0; i < cache_cap; ++i) tmp[i] = cache_vals[i];
    seq_free(cache_vals);
#else
    seq_t *tmp = (seq_t *)realloc(cache_vals, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);                 /* allocation failure */
#endif
    cache_vals = tmp;
    cache_cap  = new_cap;
}

/* Ensure cache contains values up to index n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (cache_len > n) return;         /* already have enough terms */

    /* One‑time initialization with the first two rows. */
    if (cache_len == 0) {
        /* Cache the first two sequence values a(0)=1, a(1)=2 */
        cache_cap   = 2;
        cache_vals  = seq_alloc(2);
        cache_vals[0] = (seq_t)1;     /* a(0) */
        cache_vals[1] = (seq_t)2;     /* a(1) */
        cache_len    = 2;

        /* Prepare first triangle row (row index 1) */
        prev_row     = seq_alloc(1);
        prev_row[0]  = (seq_t)2;
        prev_len     = 1;
        prev_row_idx = 1;
    }

    /* Generate subsequent rows until we have a(n) */
    while (cache_len <= n) {
        size_t new_len = prev_len + 1;                  /* length of new row */
        seq_t *row = seq_alloc(new_len);
        if (!row) exit(1);                             /* allocation failure */

        /* Compute the first element of the new row */
        seq_t first;
        if (prev_row_idx == 1) {                       /* row 2 special case */
            first = prev_row[0] + (seq_t)1;             /* 2 + 1 = 3 */
        } else {
            first = prev_row[prev_len - 1] + prev_row[0];
        }

        /* Build the row using the previous row as differences */
        row[0] = first;
        for (size_t i = 0; i < prev_len; ++i)
            row[i + 1] = row[i] + prev_row[i];

        /* Append this row's elements to the cache */
        for (size_t i = 0; i < new_len; ++i) {
            cache_grow(cache_len + 1);
            cache_vals[cache_len++] = row[i];
        }

        /* Advance to next row for subsequent generation */
        seq_free(prev_row);
        prev_row     = row;
        prev_len     = new_len;
        ++prev_row_idx;
    }
}

/* Public function expected by the build system. */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache_vals[n];
}
