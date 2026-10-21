/*
 * A084057  a(n) = 2*a(n-1) + 4*a(n-2), a(0)=1, a(1)=1.
 * https://oeis.org/A084057
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n-1) + 4*a(n-2), a(0)=1, a(1)=1
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 38, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache to store computed terms */
static seq_t *a_cache = NULL;
static idx_t a_last  = (idx_t)-1;   /* last index computed */
static size_t a_cap   = 0;          /* allocated capacity of a_cache */

/* Ensure `a_cache` has capacity for at least `need` elements */
static void ensure_cap(size_t need) {
    if (a_cap < need) {
        size_t new_cap = a_cap ? a_cap * 2 : 2;
        while (new_cap < need) new_cap *= 2;
        a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        a_cap  = new_cap;
    }
}

seq_t a(idx_t n) {
    /* Base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;

    /* Initialise cache on first call */
    if (a_cache == NULL) {
        a_cap = 2;
        a_cache = (seq_t *)malloc(a_cap * sizeof(seq_t));
        a_cache[0] = (seq_t)1;
        a_cache[1] = (seq_t)1;
        a_last = 1;
    }

    /* Return cached value if already computed */
    if (n <= a_last) return a_cache[n];

    /* Compute successive terms up to n */
    ensure_cap((size_t)n + 1);          /* capacity is at least n+1 */
    for (idx_t i = a_last + 1; i <= n; ++i) {
        a_cache[i] = (seq_t)2 * a_cache[i - 1] + (seq_t)4 * a_cache[i - 2];
    }
    a_last = n;
    return a_cache[n];
}
