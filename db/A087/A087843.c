/*
 * A087843  a(n) = a(n - a(n-1)) + a(a(a(n - a(n-2)))).
 * https://oeis.org/A087843
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 4001, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A087843
    Recurrence:  a(1) = a(2) = 1
                  a(n) = a(n - a(n-1)) + a(a(a(n - a(n-2))))
    The function memoizes results and computes all terms up to the
    requested index on demand.  Indices start at 1.  Complexity
    O(n) for the first call that reaches n, subsequent calls are
    O(1).  All intermediate and final values fit in seq_t.  */
#include <stdlib.h>

static seq_t *seq_cache = NULL;
static size_t cache_len = 0;  /* number of terms already computed (>=0) */
static size_t cache_cap = 0;  /* allocated capacity in terms */

/* Ensure that the cache contains at least `n` entries (indices 1..n). */
static void
ensure_cache(size_t n)
{
    if (cache_cap < n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 4;
        if (new_cap < n) new_cap = n;
        seq_t *new_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!new_cache) { /* allocation failed – abort */
            exit(EXIT_FAILURE);
        }
        seq_cache = new_cache;
        cache_cap = new_cap;
    }
    /* Compute any missing terms up to n */
    for (size_t i = cache_len; i < n; ++i) {          /* i is 0‑based index for a(i+1) */
        size_t idx = i + 1;                           /* 1‑based index */
        seq_t val;
        if (idx == 1 || idx == 2) {
            val = (seq_t)1;
        } else {
            /* a(idx-1) and a(idx-2) are guaranteed to exist */
            seq_t a_nm1 = seq_cache[idx - 2];          /* a(idx-1) */
            seq_t a_nm2 = seq_cache[idx - 3];          /* a(idx-2) */

            /* t1 = idx - a(idx-1) */
            size_t t1 = idx - (idx_t)a_nm1;            /* >= 1 because a_nm1 < idx */
            seq_t term1 = seq_cache[t1 - 1];

            /* t2 = idx - a(idx-2) */
            size_t t2 = idx - (idx_t)a_nm2;            /* >= 1 */
            seq_t y   = seq_cache[t2 - 1];             /* a(t2) */
            seq_t z   = seq_cache[(size_t)y - 1];      /* a(y)   */
            seq_t term2 = seq_cache[(size_t)z - 1];    /* a(z)   */

            val = term1 + term2;
        }
        seq_cache[i] = val;
    }
    cache_len = n;
}

/* Public interface */
seq_t
a(idx_t n)
{
    if (n == 0) return 0; /* undefined but safe */
    if (n > (idx_t)cache_cap) {
        ensure_cache((size_t)n);
    } else if (n > (idx_t)cache_len) {
        ensure_cache((size_t)n);
    }
    return seq_cache[(size_t)n - 1];
}
