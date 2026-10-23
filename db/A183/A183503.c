/*
 * A183503  Number of n X 1 0..3 arrays with no element equal to the sum mod 4
 *          of its horizontal and vertical neighbors.
 * https://oeis.org/A183503
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Empirical recurrence for A183503:
 *   a(1)=3, a(2)=12, a(3)=27, a(4)=81
 *   a(n)=3*a(n-1)+3*a(n-3)-9*a(n-4)   for n ≥ 5
 * This implementation keeps a memoised array that grows as needed.
 */

#include <stdlib.h>
#include <stdbool.h>

/* compute a(n) */
seq_t a(idx_t n)
{
    /* static cache: indices are 1‑based, element 0 is unused */
    static seq_t *cache = NULL;
    static idx_t cap   = 0;            /* current allocated capacity (highest usable index) */
    static idx_t len   = 0;            /* highest index already computed */
    static bool   init  = false;

    /* initialise cache with the first 4 terms */
    if (!init) {
        cap = 4;
        /* allocate cap+1 elements so that cache[cap] is valid */
        cache = (seq_t *)malloc((size_t)(cap + 1) * sizeof(seq_t));
        if (!cache) return 0;                 /* defensive, should not happen */

        cache[1] = (seq_t)3;
        cache[2] = (seq_t)12;
        cache[3] = (seq_t)27;
        cache[4] = (seq_t)81;

        len   = 4;
        init  = true;
    }

    /* if n is within the cached range, return immediately */
    if (n <= len)
        return cache[n];

    /* extend cache if necessary */
    if (n > cap) {
        idx_t new_cap = cap * 2;
        if (new_cap < n)
            new_cap = n;
        /* allocate new_cap+1 elements */
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)(new_cap + 1) * sizeof(seq_t));
        if (!tmp) return 0;                 /* allocation failure */
        cache = tmp;
        cap   = new_cap;
    }

    /* compute the remaining terms up to n */
    for (idx_t i = len + 1; i <= n; ++i) {
        seq_t a1 = cache[i - 1];
        seq_t a3 = cache[i - 3];
        seq_t a4 = cache[i - 4];
        /* recurrence: a[i] = 3*a1 + 3*a3 - 9*a4 (all terms are unsigned) */
        cache[i] = (seq_t)3 * a1 + (seq_t)3 * a3 - (seq_t)9 * a4;
    }

    len = n;      /* update highest computed index */
    return cache[n];
}
