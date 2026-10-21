/*
 * A059366  Triangle T(m,s), m >= 0, 0 <= s <= m, arising in the computation
 *          of certain integrals.
 * https://oeis.org/A059366
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabl, nonn, easy
 * Formula:   T(m,s) triangle computed by recurrence * T(m+2,s) =
 *            (2m+3)(T(m+1,s-1)+T(m+1,s)) – 4(m+1)^2T(m,s-1) * a(n) is the
 *            flattened list of T(m,s) with m>=0, 0<=s<=m
 * Range:     u8 n <= 14, u16 n <= 27, u32 n <= 65, u64 n <= 170,
 *            u128 n <= 434, big any n
 * Helpers:   alloc_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/alloc_seq_array.h"   /* alloc_seq_array(idx_t n) returns seq_t * */

static seq_t *cache      = NULL;   /* flattened sequence values      */
static size_t cache_cap  = 0;      /* allocated cache capacity        */
static size_t cache_size = 0;      /* number of values stored         */

/* Recurrence storage: only the last two rows are needed. */
static seq_t *row_prevprev = NULL; /* T(m-2, *)        */
static size_t len_prevprev = 0;
static seq_t *row_prev     = NULL; /* T(m-1, *)        */
static size_t len_prev     = 0;
static idx_t  cur_m        = 0;    /* highest m computed so far      */

/* Grow cache if needed and append a value. */
static void append_cache(seq_t val)
{
    if (cache_size + 1 > cache_cap) {
        size_t new_cap = (cache_cap > 0) ? 2 * cache_cap : 8;
        while (new_cap < cache_size + 1) new_cap *= 2;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) abort();
        cache = new_cache;
        cache_cap = new_cap;
    }
    cache[cache_size++] = val;
}

/* Produce the next row of the triangle (new_m >= 2). */
static void build_row(idx_t new_m)
{
    size_t new_len = (size_t)new_m + 1;
    seq_t *row_new = alloc_seq_array(new_len);
    if (!row_new) abort();

    seq_t coeff   = (seq_t)(2 * (new_m - 2) + 3);           /* 2(m)+3   */
    seq_t factor  = (seq_t)(new_m - 1);                     /* m+1      */
    seq_t factor2 = factor * factor;                       /* (m+1)^2  */
    seq_t four    = (seq_t)4;                               /* constant */

    for (size_t s = 0; s < new_len; ++s) {
        seq_t a = (s < len_prev)            ? row_prev[s]      : (seq_t)0;
        seq_t b = (s > 0 && s-1 < len_prev) ? row_prev[s-1]    : (seq_t)0;
        seq_t c = (s > 0 && s-1 < len_prevprev) ? row_prevprev[s-1] : (seq_t)0;

        seq_t term1 = coeff * (b + a);
        seq_t term2 = four * factor2 * c;
        row_new[s]  = term1 - term2;
    }

    /* cache the row */
    for (size_t i = 0; i < new_len; ++i) append_cache(row_new[i]);

    /* shift rows for next iteration (do not free, as seq_t may be non‑trivial). */
    row_prevprev = row_prev;
    len_prevprev = len_prev;
    row_prev     = row_new;
    len_prev     = new_len;
}

/* Initialise rows 0 and 1 and the cache. */
static void init_sequence(void)
{
    /* row 0: [1] */
    row_prevprev = alloc_seq_array(1);
    row_prevprev[0] = (seq_t)1;
    len_prevprev = 1;
    append_cache(row_prevprev[0]);

    /* row 1: [1,1] */
    row_prev = alloc_seq_array(2);
    row_prev[0] = (seq_t)1;
    row_prev[1] = (seq_t)1;
    len_prev = 2;
    append_cache(row_prev[0]);
    append_cache(row_prev[1]);

    cur_m = 1;
}

/* Return the largest m such that m(m+1)/2 <= n.  */
static idx_t row_of_n(idx_t n)
{
    idx_t low = 0, high = n;
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        __int128 tri = (__int128)mid * (mid + 1) / 2;
        if (tri <= n) low = mid; else high = mid - 1;
    }
    return low;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (cache_size == 0) init_sequence();

    if (n < cache_size) return cache[n];

    idx_t target_m = row_of_n(n);

    while (cur_m < target_m) build_row(++cur_m);

    return cache[n];
}
