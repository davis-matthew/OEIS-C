/*
 * A221454  Number of 0..3 arrays of length n with each element unequal to at
 *          least one neighbor, with new values introduced in 0..3 order.
 * https://oeis.org/A221454
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) + a(n-2) - 6*a(n-3) - 3*a(n-4), for n ≥ 5
 *            Initial values: a(1)=0, a(2)=1, a(3)=2, a(4)=7 (OEIS A221454)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 19, u64 n <= 35, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* handle non‑positive index sensibly */
    if (n == 0)
        return (seq_t)0;

    /* use size_t for array indexing (safe for n up to ~10⁴) */
    size_t N = (size_t)n;

    /* static cache of computed terms */
    static seq_t *cache = NULL;            /* pointer to array a[1..]  */
    static size_t max_cached = 0;          /* largest n for which a[n] is known */

    /* initial setup: allocate space, store base values */
    if (cache == NULL) {
        /* ensure space for at least indices 1..4 */
        size_t alloc_size = N+1;
        if (alloc_size < 5) alloc_size = 5;
        cache = (seq_t *)malloc(alloc_size * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0; /* out of memory: return 0 as a safe fallback */

        /* base values */
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)2;
        cache[4] = (seq_t)7;

        max_cached = 4;
    }

    /* grow cache if needed */
    if (N > max_cached) {
        size_t new_size = N + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;  /* out of memory */
        cache = tmp;
    }

    /* compute terms up to N if not already computed */
    if (N > max_cached) {
        for (size_t i = max_cached + 1; i <= N; ++i) {
            /* recurrence a(i) = 4*a(i-1) + a(i-2) - 6*a(i-3) - 3*a(i-4) */
            seq_t val = (seq_t)4 * cache[i-1] + cache[i-2];
            val -= (seq_t)6 * cache[i-3];
            val -= (seq_t)3 * cache[i-4];
            cache[i] = val;
        }
        max_cached = N;
    }

    /* return the requested value */
    return cache[N];
}
