/*
 * A159615  The slowest increasing sequence beginning with a(1)=2 such that
 *          a(n) and n are both odious or both not odious.
 * https://oeis.org/A159615
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=2; for n>1, a(n)=min{m>a(n-1): popcount(m)%2 ==
 *            popcount(n)%2}
 * Range:     u8 n <= 159, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Helper: compute parity of the popcount of an integer of type seq_t.
   Returns 1 if the number of 1-bits is odd (odious), 0 otherwise. */
static inline int parity_popcount(seq_t x)
{
    int p = 0;
    while (x) {
        p ^= (int)(x & (seq_t)1);
        x >>= 1;
    }
    return p;
}

/* Store computed sequence values.  cache[i] holds a(i+1). */
static seq_t *cache = NULL;
static idx_t cache_size = 0;   /* number of values stored */
static size_t cache_cap = 0;   /* allocated capacity */

/* Grow the cache to at least 'new_cap' elements, reallocating if necessary. */
static void ensure_cache(idx_t new_cap)
{
    if ((size_t)new_cap <= cache_cap)
        return;
    size_t new_capacity = cache_cap ? cache_cap * 2 : 8;
    while (new_capacity < (size_t)new_cap)
        new_capacity *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_capacity * sizeof(seq_t));
    if (!tmp) abort();  /* out of memory */
    cache = tmp;
    cache_cap = new_capacity;
}

/* Main function required by the task. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined, but safe guard */
    /* Expand cache up to index n */
    if (cache_size < n) {
        ensure_cache(n);
        /* Compute missing values */
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            seq_t candidate;
            int parity_n = parity_popcount((seq_t)i);
            if (cache_size == 0) {
                /* a(1) is defined to be 2 */
                candidate = (seq_t)2;
            } else {
                candidate = cache[cache_size - 1] + (seq_t)1;
                /* Find smallest candidate with matching parity */
                while (parity_popcount(candidate) != parity_n)
                    candidate += (seq_t)1;
            }
            cache[cache_size] = candidate;
            ++cache_size;
        }
    }
    return cache[n - 1];
}
