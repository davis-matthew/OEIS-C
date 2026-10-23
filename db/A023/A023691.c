/*
 * A023691  Numbers with exactly 9 ones in binary expansion.
 * https://oeis.org/A023691
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n-th integer (n ≥ 1) with exactly 9 ones in its
 *            binary expansion. The function builds the sequence lazily and
 *            caches the results so that random order calls are fast. It works
 *            for any seq_t type that can be assigned from a uint64_t value
 *            and supports the standard integer arithmetic oper...
 * Range:     u8 none, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;        /* cached terms */
static idx_t cache_size  = 0;      /* number currently stored */
static idx_t cache_cap   = 0;      /* allocated capacity */

/* last candidate examined; we start the search at 0 */
static uint64_t curr_candidate = 0;

/* simple popcount for uint64_t; O(number of 1-bits) */
static inline int popcount64(uint64_t x)
{
    int c = 0;
    while (x) {
        x &= x - 1;
        ++c;
    }
    return c;
}

/* ensure that cache contains at least n terms */
static void ensure_cache(idx_t n)
{
    if (n <= cache_size)
        return;

    /* grow the cache if needed */
    if (cache_cap < n) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 1;
        if (new_cap < n)
            new_cap = n;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* generate new terms until we have n of them */
    while (cache_size < n) {
        ++curr_candidate;
        if (popcount64(curr_candidate) == 9) {
            cache[cache_size++] = (seq_t)curr_candidate;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0)  /* offset is 1; behaviour for n < 1 is irrelevant */
        return 0;
    ensure_cache(n);
    return cache[n - 1];
}
