/*
 * A130019  a(n+2) = 6*a(n+1) + (-11 + n)*a(n) + (6 - 2*n)*a(n-1) for n >= 1.
 * https://oeis.org/A130019
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 15, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A130019
 * a(0) = 0 , a(1) = 0 , a(2) = 1
 * For n ≥ 3:
 *   a(n) = 6*a(n-1) + (n-13)*a(n-2) + (10-2*n)*a(n-3)
 *   (equivalent to the original recurrence:
 *     a(n+2)=6*a(n+1)+(-11+n)*a(n)+(6-2*n)*a(n-1))
 */

#include <stdlib.h>          /* realloc, abort */

static seq_t* cache = NULL;
static size_t cache_cap = 0;   /* current allocated capacity of cache */
static idx_t cache_computed = 0; /* one past the highest pre‑computed index */

static void ensure_cache_capacity(idx_t n)
{
    /* Ensure cache has room for indices 0 … n (inclusive) */
    if (cache_cap > n) return;
    size_t new_cap = cache_cap == 0 ? 16 : cache_cap;
    while (new_cap <= (size_t)n) new_cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Handle base cases directly */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;

    /* Lazy allocation and initialization of cache */
    if (cache == NULL) {
        ensure_cache_capacity(2);
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)1;
        cache_computed = 3;   /* indices 0,1,2 are now valid */
    }

    /* Ensure the cache is large enough for index n */
    if (n >= cache_cap) {
        ensure_cache_capacity(n);
    }

    /* Compute terms up to n if necessary */
    if (cache_computed <= n) {
        for (idx_t idx = cache_computed; idx <= n; ++idx) {
            /* idx ≥ 3 at this point */
            seq_t term1 = (seq_t)6 * cache[(size_t)(idx - 1)];
            seq_t pos2 = 0, sub2 = 0;
            seq_t pos3 = 0, sub3 = 0;

            int coeff2 = (int)idx - 13;   /* coefficient of a(idx-2) */
            if (coeff2 >= 0) {
                pos2 = (seq_t)coeff2 * cache[(size_t)(idx - 2)];
            } else {
                sub2 = (seq_t)(-coeff2) * cache[(size_t)(idx - 2)];
            }

            int coeff3 = 10 - 2 * (int)idx; /* coefficient of a(idx-3) */
            if (coeff3 >= 0) {
                pos3 = (seq_t)coeff3 * cache[(size_t)(idx - 3)];
            } else {
                sub3 = (seq_t)(-coeff3) * cache[(size_t)(idx - 3)];
            }

            seq_t val = term1;
            val += pos2;
            val -= sub2;
            val += pos3;
            val -= sub3;

            cache[(size_t)idx] = val;
        }
        cache_computed = n + 1;
    }

    return cache[(size_t)n];
}
