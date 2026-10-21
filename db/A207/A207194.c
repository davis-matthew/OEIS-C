/*
 * A207194  Triangular array read by rows. T(n,k) is the number of
 *          compositions of the integer k into at most n summands, each of
 *          which is at most n, n >= 0, k >= 0.
 * https://oeis.org/A207194
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   sum_row_lengths
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A207194: flattened triangle of compositions.  a(n) is the
   n-th element (0‑based) of the array formed by reading the triangle
   T(n,k) by rows, where T(n,k) counts compositions of k into at most
   n parts, each part ≤ n.  */
#include <stdlib.h>
#include "../utils/sum_row_lengths.h"

static void compute_row(idx_t r, seq_t **rows)
{
    idx_t len = r * r + 1;
    seq_t *row  = (seq_t*)calloc((size_t)len, sizeof(seq_t));
    seq_t *dp_prev = (seq_t*)calloc((size_t)len, sizeof(seq_t));
    seq_t *dp_cur  = (seq_t*)calloc((size_t)len, sizeof(seq_t));
    seq_t *pref    = (seq_t*)calloc((size_t)len, sizeof(seq_t));

    row[0] = 1;
    dp_prev[0] = 1;

    for (idx_t parts = 1; parts <= r; ++parts)
    {
        pref[0] = dp_prev[0];
        for (idx_t s = 1; s < len; ++s) pref[s] = pref[s - 1] + dp_prev[s];

        for (idx_t s = 1; s < len; ++s)
        {
            seq_t total = pref[s - 1];
            if (s > r) total -= pref[s - r - 1];
            dp_cur[s] = total;
        }
        dp_cur[0] = 0;   /* no composition of >0 parts that sums to 0 */

        for (idx_t s = 1; s < len; ++s) row[s] += dp_cur[s];

        seq_t *tmp = dp_prev;
        dp_prev = dp_cur;
        dp_cur  = tmp;
    }

    free(dp_cur);
    free(dp_prev);
    free(pref);

    rows[r] = row;
}

static seq_t **row_cache = NULL;
static idx_t   cache_cap = 0;

static void ensure_cache_capacity(idx_t r)
{
    if (r < cache_cap) return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap <= r) new_cap <<= 1;
    seq_t **tmp = (seq_t**)realloc(row_cache, (size_t)new_cap * sizeof(seq_t*));
    for (idx_t i = cache_cap; i < new_cap; ++i) tmp[i] = NULL;
    row_cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    idx_t r = 0;
    while (sum_row_lengths(r) <= n) ++r;

    idx_t prev_len = (r == 0) ? 0 : (idx_t)sum_row_lengths(r - 1);
    idx_t k = n - prev_len;

    ensure_cache_capacity(r);
    if (row_cache[r] == NULL) compute_row(r, row_cache);

    return row_cache[r][k];
}
