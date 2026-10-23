/*
 * A014805  Expansion of Jacobi theta constant (theta_2/2)^16.
 * https://oeis.org/A014805
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 45, u64 n <= 1138,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A014805: number of ways of writing n as a sum of 16 triangular numbers
 * (triangular numbers include 0).  The value is the coefficient of q^n in
 *  (∑_{t triangular} q^t)^16.
 *
 * Recurrence approach: for each n we convolve the current sequence with the
 * indicator of triangular numbers 16 times.  This gives an O(max_n·T) algorithm,
 * where T ≈ √(2·max_n) is the number of triangular numbers ≤ max_n.
 * The implementation memoizes results so that subsequent calls with
 * larger n reuse the already‑computed table.
 */

#include <stdlib.h>
#include <string.h>

/* The types seq_t and idx_t are supplied by the build system.  */

static seq_t *cache = NULL;   /* cache[0..max_n] */
static size_t cache_cap = 0;  /* current capacity = cache_cap */

/* Pre‑compute a(0)..a(max_n) by 16 successive convolutions with the
 * triangular‑number series  Σ q^t.  The algorithm is
 *    cache ← {1,0,0,…,0}
 *    for pass = 1 … 16
 *        next ← array of zeros
 *        for s = 0 … max_n
 *            for each triangular t ≤ max_n−s
 *                next[s+t] += cache[s]
 *        cache ← next
 */
static void recompute_cache(idx_t max_n)
{
    size_t cap = (size_t)max_n + 1;

    /* free previous cache */
    free(cache);
    cache = (seq_t *)calloc(cap, sizeof(seq_t));
    if (!cache) return;          /* allocation failure */

    /* temporary array for the next convolution */
    seq_t *next = (seq_t *)calloc(cap, sizeof(seq_t));
    if (!next) { free(cache); cache = NULL; return; }

    /* initial sequence: only coefficient of q^0 is 1 */
    cache[0] = (seq_t)1;

    /* collect all triangular numbers ≤ max_n (including 0) */
    idx_t tri[200];
    size_t tri_cnt = 0;
    for (idx_t r = 0; ; ++r) {
        idx_t t = r * (r + 1) / 2;
        if (t > max_n) break;
        tri[tri_cnt++] = t;
    }

    /* perform 16 convolutions */
    for (idx_t pass = 0; pass < 16; ++pass) {
        /* zero the next array */
        memset(next, 0, cap * sizeof(seq_t));

        for (idx_t s = 0; s <= max_n; ++s) {
            seq_t cur = cache[s];
            if (cur == (seq_t)0) continue;
            for (size_t k = 0; k < tri_cnt; ++k) {
                idx_t ns = s + tri[k];
                if (ns > max_n) break;
                next[ns] += cur;
            }
        }

        /* swap cache and next for the next iteration */
        seq_t *tmp = cache;
        cache   = next;
        next    = tmp;
    }

    /* free the old temporary array that now holds the previous cache */
    free(next);
    cache_cap = cap;
}

/* Public entry point required by the problem statement.   The function
 * can be called with indices in any order.  The implementation keeps a
 * static cache that is grown as necessary.
 */
seq_t a(idx_t n)
{
    if (n >= cache_cap)               /* need more terms */
        recompute_cache(n);
    return cache[n];
}
