/*
 * A223481  Rolling icosahedron face footprints: number of 3 X n 0..19 arrays
 *          starting with 0 where 0..19 label faces of an icosahedron and
 *          every array movement to a horizontal or antidiagonal neighbor
 *          moves across an icosahedral edge.
 * https://oeis.org/A223481
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) - 2*a(n-2) for n > 4 with initial values
 *            a(1)=400, a(2)=243, a(3)=2025, a(4)=16875
 * Range:     u8 none, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 40,
 *            big any n
 * Validated: standard, pass, 34 terms through n = 181 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

static seq_t *cache = NULL;
static idx_t cache_max = 0;   /* highest n for which cache[n] is computed */
static size_t cache_cap = 0;  /* allocated capacity (>= cache_max+1) */

seq_t a(idx_t n)
{
    /* Fast case for the first four terms */
    if (n == 1) return (seq_t)400;
    if (n == 2) return (seq_t)243;
    if (n == 3) return (seq_t)2025;
    if (n == 4) return (seq_t)16875;

    /* Allocate or grow the cache if necessary */
    if (cache == NULL) {
        cache_cap = (size_t)n + 5;               /* some spare room */
        cache = seq_alloc(cache_cap);
    } else if (cache_cap <= (size_t)n) {
        size_t new_cap = cache_cap * 2;
        if (new_cap <= (size_t)n) new_cap = (size_t)n + 5;
        cache = seq_realloc(cache, cache_cap, new_cap);
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t val;
        if (i == 1) val = (seq_t)400;
        else if (i == 2) val = (seq_t)243;
        else if (i == 3) val = (seq_t)2025;
        else if (i == 4) val = (seq_t)16875;
        else {
            /* recurrence: a(i) = 9*a(i-1) - 2*a(i-2) */
            seq_t prev1 = cache[(size_t)(i - 1)];
            seq_t prev2 = cache[(size_t)(i - 2)];
            val = (seq_t)(9 * prev1 - 2 * prev2);
        }
        cache[(size_t)i] = val;
    }
    cache_max = n;
    return cache[(size_t)n];
}
