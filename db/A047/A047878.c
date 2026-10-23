/*
 * A047878  a(n) is the least number of knight's moves from corner (0,0) to
 *          n-th diagonal of unbounded chessboard.
 * https://oeis.org/A047878
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-3) - a(n-4) (n > 5) initial values: a(0)=0,
 *            a(1)=3, a(2)=2, a(3)=1, a(4)=2, a(5)=3 (A047878 – least knight
 *            moves to the n‑th diagonal)
 * Range:     u8 n <= 763, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_realloc.h"   /* seq_realloc(seq_t *old, size_t old_cap, size_t new_cap) */

static seq_t *cache = NULL;          /* array of computed terms */
static size_t cache_cap   = 0;        /* allocated capacity (in seq_t elements) */
static size_t cache_size  = 0;        /* number of terms actually stored */

/* Ensure that terms 0 .. n are computed and stored in 'cache'. */
static void
ensure_cache(idx_t n)
{
    if ((size_t)n < cache_size) return;  /* already available */

    /* Minimum capacity needed to start the recurrence. */
    size_t needed_cap = (size_t)n + 1;
    if (needed_cap < 8) needed_cap = 8;      /* safety for early terms */

    if (needed_cap > cache_cap) {           /* grow the array */
        seq_t *tmp = seq_realloc(cache, cache_cap, needed_cap);
        if (!tmp) abort();                  /* allocation failure */
        cache      = tmp;
        cache_cap  = needed_cap;
    }

    /* Compute new terms, starting from the first missing index. */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i == 0)      cache[i] = (seq_t)0;
        else if (i == 1) cache[i] = (seq_t)3;
        else if (i == 2) cache[i] = (seq_t)2;
        else if (i == 3) cache[i] = (seq_t)1;
        else if (i == 4) cache[i] = (seq_t)2;
        else if (i == 5) cache[i] = (seq_t)3;
        else             cache[i] = cache[i-1] + cache[i-3] - cache[i-4];
    }

    cache_size = (size_t)n + 1;
}

/* Public function computing the n‑th term of A047878 */
seq_t
a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
