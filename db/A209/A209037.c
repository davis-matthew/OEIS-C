/*
 * A209037  Number of partitions of n into parts not less than max(1,n/3).
 * https://oeis.org/A209037
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into parts not less than max(1,
 *            ⌊n/3⌋)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses dynamic programming in O(n²) time, storing
   intermediate results in a static cache so that the function can be
   called in any order.  It is written to compile as both C11 and
   C++17.  The malloc/calloc return values are cast explicitly to the
   required pointer types.  No use of 'new', 'class', or templates is
   made. */

#include <stdlib.h>

/* ------------------------------------------------------------------ */
/*  Cache of previously computed values.  cache[i] holds a(i) for
    0 ≤ i ≤ max_n.  The array is rebuilt whenever a larger n is
    requested.                                      */
static seq_t *cache = NULL;          /* cache[i] = a(i) for 0 ≤ i ≤ max_n */
static idx_t  max_n  = 0;            /* highest index currently cached */

/* ------------------------------------------------------------------ */
/*  Compute all entries a(0)…a(n) (n ≥ 1).  The algorithm maintains a
    single row of the recurrence

        p(n,ℓ) = p(n,ℓ+1) + p(n−ℓ,ℓ)   (ℓ ≤ n)

    where p(n,ℓ) is the number of partitions of n with all parts
    ≥ ℓ.  The required value is a(n)=p(n,m(n)) with
    m(n)=max(1,⌊n/3⌋). */
static void compute_until(idx_t n)
{
    if (n <= max_n) return;         /* already computed up to n */

    /* Allocate temporary arrays for the DP. */
    seq_t *dp_next = (seq_t*)calloc((size_t)(n + 1), sizeof(seq_t));
    seq_t *dp_cur  = (seq_t*)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!dp_next || !dp_cur) return;   /* allocation failure – abort */

    /* p(0,ℓ)=1 for all ℓ. */
    dp_next[0] = (seq_t)1;

    /* m[i] = max(1, ⌊i/3⌋) for all i ≤ n. */
    idx_t *m = (idx_t*)malloc((size_t)(n + 1) * sizeof(idx_t));
    if (!m) {
        free(dp_next); free(dp_cur);
        return;
    }
    for (idx_t i = 0; i <= n; ++i)
        m[i] = (i < 3 ? (idx_t)1 : i / 3);

    /* Allocate a new cache to hold the results. */
    seq_t *new_cache = (seq_t*)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!new_cache) {
        free(dp_next); free(dp_cur); free(m);
        return;
    }

    /* Run the recurrence from ℓ = n down to 1.  For each ℓ we compute
       dp_cur = p(·,ℓ).                                            */
    for (idx_t l = n; l; --l) {
        dp_cur[0] = dp_next[0];
        for (idx_t i = 1; i <= n; ++i) {
            seq_t val = dp_next[i];
            if (i >= l) val += dp_cur[i - l];
            dp_cur[i] = val;
        }

        /* Whenever the current ℓ equals the required minimum part for
           some i, store that value in the cache. */
        for (idx_t i = 0; i <= n; ++i)
            if (m[i] == l)
                new_cache[i] = dp_cur[i];

        /* Prepare for the next iteration. */
        seq_t *tmp = dp_next;
        dp_next = dp_cur;
        dp_cur  = tmp;
    }

    /* Free temporary buffers. */
    free(dp_next); free(dp_cur); free(m);

    /* Replace the old cache with the new one. */
    if (cache) free(cache);
    cache = new_cache;
    max_n = n;
}

/* ------------------------------------------------------------------ */
/*  Public interface required by the framework. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;    /* by definition a(0) = 1 */
    compute_until(n);
    return cache[n];
}
