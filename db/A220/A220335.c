/*
 * A220335  A modified Engel expansion for sqrt(3) - 1.
 * https://oeis.org/A220335
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 13, u32 n <= 16, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:
 *   a(1) = 2
 *   a(3*k+1) = 2                              (k ≥ 0)
 *   a(3*k+2) = 2 * (a(3*(k-1)+2)^2 - 2*a(3*(k-1)+2) + 1)   (k ≥ 1)
 *   a(3*k  ) = 2*a(3*k-1) - 2                 (k ≥ 1)
 *
 *  The function returns the n‑th term of the sequence for any n ≥ 1 and
 *  may be called in any order.  Results are cached to avoid recomputing
 *  previous terms.  Allocation is performed once on first use and is
 *  grown only if n exceeds the current capacity.
 */
#include <stdlib.h>
#include <stddef.h>

static seq_t *a_cache = NULL;   /* 1‑based cache, 0 index unused */
static idx_t  a_max   = 0;      /* highest index computed so far */
static idx_t  a_cap   = 0;      /* allocated capacity (size of a_cache) */

/* Ensure that a_cache[n] is available for all n ≤ limit. */
static void ensure_up_to(idx_t limit)
{
    if (limit <= a_max)
        return;

    /* Allocate or grow the cache. */
    if (a_cache == NULL) {
#if defined(__cplusplus)
        a_cache = new seq_t[(size_t)limit + 1];
#else
        a_cache = (seq_t *)malloc(((size_t)limit + 1) * sizeof(seq_t));
#endif
        if (!a_cache) abort();
        a_cap = limit + 1;
    } else if (limit + 1 > a_cap) {
#if defined(__cplusplus)
        seq_t *new_cache = new seq_t[(size_t)limit + 1];
        for (idx_t i = 1; i <= a_max; ++i)
            new_cache[i] = a_cache[i];
        delete[] a_cache;
        a_cache = new_cache;
#else
        a_cache = (seq_t *)realloc(a_cache,
            ((size_t)limit + 1) * sizeof(seq_t));
        if (!a_cache) abort();
#endif
        a_cap = limit + 1;
    }

    /* Compute missing terms. */
    for (idx_t i = a_max + 1; i <= limit; ++i) {
        seq_t val;
        if (i == 1) {
            val = (seq_t)2;
        } else {
            idx_t r = i % 3;
            if (r == 1) {
                val = (seq_t)2;
            } else if (r == 2) {
                /* a(3*k+2) with k ≥ 1 for i ≥ 5; handle i==2 separately */
                if (i == 2) {
                    val = (seq_t)3;
                } else {
                    seq_t prev = a_cache[i - 3]; /* a(3*(k-1)+2) */
                    seq_t t = prev * prev;      /* prev^2 */
                    t -= (seq_t)2 * prev;        /* -2*prev */
                    t += (seq_t)1;               /* +1 */
                    val = (seq_t)2 * t;          /* *2 */
                }
            } else { /* r == 0, i = 3*k, k ≥ 1 */
                seq_t prev = a_cache[i - 1];    /* a(3*k-1) */
                val = (seq_t)2 * prev - (seq_t)2;
            }
        }
        a_cache[i] = val;
    }
    a_max = limit;
}

seq_t a(idx_t n)
{
    ensure_up_to(n);
    return a_cache[n];
}
