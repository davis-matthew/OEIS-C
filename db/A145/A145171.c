/*
 * A145171  Triangle read by rows: left half of trinomial triangle (A027907)
 *          modulo 3.
 * https://oeis.org/A145171
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the (row, offset) entry of the trinomial triangle modulo
 *            3, where the row index r and offset k satisfy n = r(r+1)/2 + k +
 *            1, 0 ≤ k ≤ r
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache       = NULL; /* flattened sequence values (mod 3)          */
static size_t cache_size  = 0;    /* number of values currently stored         */
static size_t cache_cap   = 0;    /* allocated capacity of cache array          */
static seq_t *full_row    = NULL; /* full current row of the trinomial triangle */
static size_t full_len    = 0;    /* length of full_row (2*r + 1)              */
static size_t row_index   = 0;    /* index of the current row (starts at 0)     */

/* Ensure that at least n entries of the sequence are generated and stored. */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        /* initialise the first row: r = 0, full_row[0] = 1 */
        cache_cap   = 64;
        cache       = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        full_row    = (seq_t *)malloc(sizeof(seq_t));
        full_row[0] = (seq_t)1;
        full_len    = 1;
        row_index   = 0;
    }

    const seq_t MOD3 = (seq_t)3; /* constant for mod 3 operations */

    while ((idx_t)cache_size < n) {
        /* ----- append the left‑half of the current row to the cache ----- */
        size_t left_len = row_index + 1;               /* length of left half */
        if (cache_size + left_len > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 64;
            while (new_cap < cache_size + left_len)
                new_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return; /* allocation failure – give up */
            cache = tmp;
            cache_cap = new_cap;
        }

        for (size_t i = 0; i < left_len; ++i)
            cache[cache_size + i] = full_row[i];
        cache_size += left_len;

        /* ----- compute the next full row of the trinomial triangle ----- */
        size_t next_len = full_len + 2;          /* 2*(row+1)+1 */
        seq_t *next = (seq_t *)malloc(next_len * sizeof(seq_t));
        if (!next) return;                      /* allocation failure – give up */

        for (size_t j = 0; j < next_len; ++j) {
            seq_t val = (seq_t)0;
            if (j < full_len)          val += full_row[j];
            if (j > 0 && j - 1 < full_len)  val += full_row[j - 1];
            if (j > 1 && j - 2 < full_len)  val += full_row[j - 2];
            next[j] = val % MOD3;
        }

        free(full_row);
        full_row = next;
        full_len = next_len;
        ++row_index;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)          /* index 0 is outside the OEIS offset – not used */
        return (seq_t)0;

    ensure_cache(n);
    return cache[(size_t)(n - 1)];
}
