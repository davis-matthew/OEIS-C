/*
 * A136579  Triangle read by rows: A128174 * A136572.
 * https://oeis.org/A136579
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = j! if row r and column j have the same parity; otherwise
 *            a(n) = 0. Where r = floor((sqrt(8n+1)-1)/2) (the row index,
 *            starting from 0) and j = n - r(r+1)/2 (the column index within
 *            that row)
 * Range:     u8 n <= 26, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *fact_cache = NULL;   /* fact_cache[k] = k!  (k ≤ fact_cap-1) */
static idx_t fact_cap = 0;

/* Grow the factorial cache so that it contains factorials up to m (inclusive). */
static void ensure_fact(idx_t m)
{
    if (m < fact_cap)   /* already computed */
        return;

    idx_t new_cap = m + 1;
    seq_t *new_cache = (seq_t *)realloc(fact_cache, new_cap * sizeof(seq_t));
    if (!new_cache)      /* allocation failed – leave cache unchanged */
        return;

    /* If the cache was empty, initialise 0! = 1. */
    if (fact_cap == 0)
        new_cache[0] = (seq_t)1;

    /* Compute new factorial entries.  Start from the first index that was
       not yet present.  Since realloc preserves old data, new_cache[0..fact_cap-1]
       already contain the old values. */
    idx_t start = (fact_cap == 0 ? 1 : fact_cap);
    for (idx_t k = start; k <= m; ++k)
        new_cache[k] = new_cache[k - 1] * (seq_t)k;

    fact_cache = new_cache;
    fact_cap   = new_cap;
}

seq_t a(idx_t n)
{
    /* Find the row r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2. */
    idx_t high = 1;
    while ((unsigned __int128)high * (high + 1) / 2 <= n)
        high <<= 1;
    idx_t low = high >> 1;

    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri <= n)
            low = mid;
        else
            high = mid - 1;
    }
    idx_t r = low;

    idx_t prev_tri = (idx_t)((unsigned __int128)r * (r + 1) / 2);
    idx_t j = n - prev_tri;    /* column index in the row */

    /* Zero if the parity of row and column differ. */
    if ((r & 1) != (j & 1))
        return (seq_t)0;

    /* Return j! (ensure it is cached). */
    ensure_fact(j);
    if (fact_cache == NULL)    /* allocation failure – return 0 */
        return (seq_t)0;
    return fact_cache[j];
}
