/*
 * A077413  Bisection (odd part) of Chebyshev sequence with Diophantine
 *          property.
 * https://oeis.org/A077413
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - a(n-2), a(0)=2, a(1)=13
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses a static cache to store previously computed
 * values so that calls arrive in any order and the sequence is computed
 * in O(n) time overall.  The cache grows as needed.  All arithmetic is
 * performed using the type seq_t, which may be an integer type or an
 * arbitrary‑precision integer class. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the two base cases directly */
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)13;

    /* Static cache for the sequence values */
    static seq_t *cache = NULL;   /* array of seq_t */
    static idx_t allocated = 0;   /* number of elements allocated */
    static idx_t computed = 0;    /* number of elements computed (highest index+1) */

    /* Ensure we have space for indices 0..n */
    if (n + 1 > allocated) {
        idx_t new_alloc = (n + 1) > (2 * allocated) ? (n + 1) : (2 * allocated);
        if (new_alloc == 0) new_alloc = 2;  /* first allocation */
        seq_t *tmp = (seq_t *)realloc(cache, new_alloc * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* allocation failed, return zero (unlikely in tests) */
        cache = tmp;
        allocated = new_alloc;
    }

    /* Initialize base values if not already done */
    if (computed == 0) {
        cache[0] = (seq_t)2;
        computed = 1;
    }
    if (computed == 1) {
        cache[1] = (seq_t)13;
        computed = 2;
    }

    /* Compute values up to index n */
    for (idx_t i = computed; i <= n; ++i) {
        seq_t next = cache[i-1];
        next = next * (seq_t)6 - cache[i-2];
        cache[i] = next;
    }
    computed = n + 1;   /* now we have up to index n computed */

    return cache[n];
}
