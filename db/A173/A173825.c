/*
 * A173825  List of numbers k such that k=6*r+1=m*q, m>=q and n>=g where
 *          number m is 6*n+1 or 6*n-1 and number q is 6*g+1 or 6*g-1.
 * https://oeis.org/A173825
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A173825: a(n) is the n‑th term of the flattened triangle
   where row r (r≥0) consists of all numbers of the form k = m*q,
   with m = 6r+1 or 6r-1, q = 6g+1 or 6g-1 (0≤g≤r),
   m≥q, and m ≡ q (mod 6).  The resulting k values are sorted
   ascending within each row before being appended to the global list. */

#include <stdlib.h>

static seq_t  *seq_cache   = NULL;  /* storage for generated terms   */
static size_t  cache_cap   = 0;     /* allocated capacity           */
static size_t  cache_len   = 0;     /* terms currently stored       */
static idx_t   next_row    = 0;     /* next row index to generate   */

/* Reallocate cache to hold at least 'needed' more elements.  */
static void
ensure_cache_cap(size_t needed)
{
    if (cache_cap - cache_len < needed) {
        size_t new_cap = cache_cap ?: 16;
        while (new_cap - cache_len < needed) {
            new_cap *= 2;
        }
        seq_cache = (seq_t *)realloc(seq_cache, sizeof(seq_t) * new_cap);
        cache_cap = new_cap;
    }
}

/* Append a sorted list of values into the global cache.  */
static void
append_row(seq_t *row, size_t len)
{
    ensure_cache_cap(len);
    for (size_t i = 0; i < len; ++i) {
        seq_cache[cache_len++] = row[i];
    }
}

/* Generate the next row of the triangle and append its elements. */
static void
generate_next_row(void)
{
    idx_t r = next_row;
    /* Prepare local array for this row.  Size <= 2*r+1. */
    size_t len = 0;
    size_t maxlen = 2 * r + 1;
    seq_t *row = (seq_t *)malloc(sizeof(seq_t) * maxlen);

    seq_t m1 = (seq_t)6 * r + 1;
    seq_t m2 = r > 0 ? (seq_t)6 * r - 1 : 0;   /* 0 marks non‑existent */

    for (idx_t g = 0; g <= r; ++g) {
        seq_t q1 = (seq_t)6 * g + 1;
        seq_t q2 = g > 0 ? (seq_t)6 * g - 1 : 0;  /* 0 marks non‑existent */

        /* Check all combinations of (m,q). */
        if (m1 >= q1 && (m1 % 6 == q1 % 6)) {
            row[len++] = m1 * q1;
        }
        if (q2 && m1 >= q2 && (m1 % 6 == q2 % 6)) {
            row[len++] = m1 * q2;
        }
        if (m2) {
            if (m2 >= q1 && (m2 % 6 == q1 % 6)) {
                row[len++] = m2 * q1;
            }
            if (q2 && m2 >= q2 && (m2 % 6 == q2 % 6)) {
                row[len++] = m2 * q2;
            }
        }
    }

    /* Insertion sort: row size is at most ~200, so trivial cost. */
    for (size_t i = 1; i < len; ++i) {
        seq_t key = row[i];
        size_t j = i;
        while (j > 0 && row[j - 1] > key) {
            row[j] = row[j - 1];
            --j;
        }
        row[j] = key;
    }

    append_row(row, len);
    free(row);
    ++next_row;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1, behaviour below offset is irrelevant */

    while (cache_len < (size_t)n) {
        generate_next_row();
    }
    return seq_cache[(size_t)n - 1];
}
