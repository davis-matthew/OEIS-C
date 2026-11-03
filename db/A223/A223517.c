/*
 * A223517  Triangle T(n,k) represents the coefficients of (x^15*d/dx)^n,
 *          where n=1,2,3,...; generalization of Stirling numbers of second
 *          kind A008277, Lah-numbers A008297.
 * https://oeis.org/A223517
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 21, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Validated: partial, pass, 36 terms through n = 36 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  T(1,1)=1;
   T(n,k)=T(n-1,k-1)+((n-1)*14+k)*T(n-1,k),   for n>1.
   Sequence a(n) is the row‑wise flattening of the triangle T. */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../utils/mem.h"

/* C allocation helpers */
static seq_t* seq_alloc_new_array(size_t n)   { return mem_new(seq_t, n); }

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

static void seq_free_array(seq_t* p)          { mem_free(p); }

static seq_t *cache = NULL;        /* flattened triangle */
static size_t cache_len = 0;       /* number of stored elements   */
static size_t cache_cap = 0;       /* allocated capacity          */

static seq_t *prev_row = NULL;     /* previous row values (1‑based) */
static size_t prev_len = 0;        /* length of prev_row (= current row number) */

/* Ensure the cache can hold at least 'need' more elements. */
static void ensure_cache_cap(size_t need)
{
    if (cache_len + need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap : 64;
    while (new_cap < cache_len + need) new_cap <<= 1;
    seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
}

/* Compute the next row and append it to the cache. */
static void compute_next_row(void)
{
    size_t row = prev_len + 1;
    size_t cur_len = row;                     /* number of elements in this row */
    seq_t *cur = seq_alloc_new_array(cur_len + 1);  /* 1‑based index; cur[0] unused */
    if (!cur) abort();

    if (row == 1) {
        cur[1] = (seq_t)1;
    } else {
        for (size_t k = 1; k <= cur_len; ++k) {
            seq_t term_left  = (k > 1)          ? prev_row[k - 1] : (seq_t)0;
            seq_t term_right = (k <= prev_len) ? prev_row[k]     : (seq_t)0;
            seq_t factor = (seq_t)(row - 1) * (seq_t)14 + (seq_t)k;
            cur[k] = term_left + term_right * factor;
        }
    }

    /* Append this row to the cache. */
    ensure_cache_cap(cur_len);
    for (size_t i = 1; i <= cur_len; ++i) {
        cache[cache_len++] = cur[i];
    }

    /* Prepare for next iteration. */
    if (prev_row) seq_free_array(prev_row);
    prev_row = cur;
    prev_len = row;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n = 0 */
    while (cache_len < (size_t)n)
        compute_next_row();
    return cache[n - 1];
}
