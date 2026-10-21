/*
 * A220611  Number of ways to reciprocally link elements of an n X 5 array
 *          either to themselves or to exactly two horizontal and vertical
 *          neighbors, without consecutive collinear links.
 * https://oeis.org/A220611
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 34, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence a(n): Number of ways to reciprocally link elements of an n × 5 array
   either to themselves or to exactly two horizontal and vertical neighbors,
   without consecutive collinear links.
   Empirical recurrence (offset 1):
        a(n) = 3*a(n-1) + 6*a(n-2) - 11*a(n-3) - 5*a(n-4) + 6*a(n-5)
   Initial values:
        a(1)=1, a(2)=8, a(3)=21, a(4)=95, a(5)=324
*/
#include <stdlib.h>

static seq_t *cache = NULL;        /* dynamic array of computed terms */
static idx_t cache_cap = 0;        /* allocated capacity (terms)       */
static idx_t computed = 0;         /* number of terms actually stored */

/* Ensure that ‘cache’ can hold at least ‘need’ terms.  */
static void
ensure_cache(idx_t need)
{
    if (cache_cap >= need)
        return;
    idx_t new_cap = need > 16 ? need * 2 : 16;
    seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
    if (!tmp)
        abort();
    cache = tmp;
    cache_cap = new_cap;
}

/* External linkage function requested by the build system. */
seq_t a(idx_t n)
{
    if (n == 0)              /* out‑of‑range guard – not used by callers   */
        return (seq_t)0;

    /* Initialise cache on the first call. */
    if (!cache) {
        ensure_cache(16);
        cache[0] = (seq_t)1;       /* a(1) */
        cache[1] = (seq_t)8;       /* a(2) */
        cache[2] = (seq_t)21;      /* a(3) */
        cache[3] = (seq_t)95;      /* a(4) */
        cache[4] = (seq_t)324;     /* a(5) */
        computed = 5;
    }

    /* Compute new terms if they are needed. */
    if (n > computed) {
        ensure_cache(n);
        /* Recurrence: a(k+1) = 3*a(k) + 6*a(k-1) - 11*a(k-2) - 5*a(k-3) + 6*a(k-4) */
        for (idx_t i = computed; i < n; ++i) {     /* i is zero‑based index for a(i+1) */
            /* All indices i−1 … i−5 are valid because i ≥ 5. */
            cache[i] = 3 * cache[i-1] + 6 * cache[i-2]
                       - 11 * cache[i-3] - 5 * cache[i-4]
                       + 6 * cache[i-5];
        }
        computed = n;
    }

    /* cache[0] holds a(1), so return the term at index n‑1. */
    return cache[n-1];
}
