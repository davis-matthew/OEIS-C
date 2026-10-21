/*
 * A099860  A Chebyshev transform related to the knot 7_1.
 * https://oeis.org/A099860
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(0)=1, a(1)=1, a(2)=2, a(3)=2, a(4)=1, a(5)=1 For n≥6: a(n) =
 *            a(n-1) – a(n-2) + a(n-3) – a(n-4) + a(n-5) – a(n-6) (this
 *            follows from the rational generating function (1+x²)² /
 *            (1‑x+x²‑x³+x⁴‑x⁵+x⁶) )
 * Range:     i8 n <= 80, i16 n <= 80, i32 n <= 80, i64 n <= 80, i128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *seq_cache = NULL;          /* pointer to cached terms          */
static size_t  cache_cap  = 0;           /* allocated size of seq_cache      */
static size_t  cache_len  = 0;           /* number of terms stored (highest
                                            index stored + 1)              */

static void ensure_cache_idx (idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (seq_cache == NULL) {
        /* first allocation */
#ifdef __cplusplus
        seq_cache = new seq_t[needed];
#else
        seq_cache = (seq_t *)malloc(needed * sizeof(seq_t));
#endif
        cache_cap = needed;
        cache_len = 0;
        return;
    }
    if (needed <= cache_cap) return;

    /* grow cache */
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < needed) new_cap <<= 1;
#ifdef __cplusplus
    seq_t *new_cache = new seq_t[new_cap];
    /* copy existing values */
    for (size_t i = 0; i < cache_len; ++i) {
        new_cache[i] = seq_cache[i];
    }
    delete [] seq_cache;
    seq_cache = new_cache;
#else
    seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
#endif
    cache_cap = new_cap;
}

static void grow_cache_to (size_t goal)
{
    if (goal <= cache_cap) return;
#ifdef __cplusplus
    seq_t *new_cache = new seq_t[goal];
    for (size_t i = 0; i < cache_len; ++i) {
        new_cache[i] = seq_cache[i];
    }
    delete [] seq_cache;
    seq_cache = new_cache;
#else
    seq_cache = (seq_t *)realloc(seq_cache, goal * sizeof(seq_t));
#endif
    cache_cap = goal;
}

seq_t a(idx_t n)
{
    /* ensure cache is large enough */
    ensure_cache_idx(n);

    /* compute new values only if needed */
    if (cache_len <= (size_t)n) {
        /* guarantee the underlying storage holds all indices up to n */
        grow_cache_to((size_t)n + 1);

        for (idx_t i = (idx_t)cache_len; i <= n; ++i) {
            seq_t v;
            if (i == 0)        v = 1;
            else if (i == 1)   v = 1;
            else if (i == 2)   v = 2;
            else if (i == 3)   v = 2;
            else if (i == 4)   v = 1;
            else if (i == 5)   v = 1;
            else { /* recurrence for i >= 6 */
                /* a(i) = a(i-1) - a(i-2) + a(i-3) - a(i-4) + a(i-5) - a(i-6) */
                v = seq_cache[(size_t)(i-1)] - seq_cache[(size_t)(i-2)];
                v = v + seq_cache[(size_t)(i-3)];
                v = v - seq_cache[(size_t)(i-4)];
                v = v + seq_cache[(size_t)(i-5)];
                v = v - seq_cache[(size_t)(i-6)];
            }
            seq_cache[(size_t)i] = v;
        }
        cache_len = (size_t)n + 1;
    }
    return seq_cache[(size_t)n];
}
