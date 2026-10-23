/*
 * A133678  a(n)=6*a(n-1)+42*a(n-2) for n>=3, a(0)=1, a(1)=6, a(2)=72 .
 * https://oeis.org/A133678
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 6*a(n-1)+42*a(n-2), a(0)=1, a(1)=6, a(2)=72
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mul_small.h"

/* Recursive sequence computation with memoisation.  The function
 * works for any integer type returned as seq_t and for any
 * arbitrary‑precision type supplied by the build.  It can be called
 * with arguments in any order. */
seq_t a(idx_t n)
{
    /* Handle the initial values directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)6;
    if (n == 2) return (seq_t)72;

    /* Static buffer for memoisation.  The buffer grows only when
     * a larger index is required. */
    static seq_t *cache = NULL;
    static idx_t  capacity = 0;      /* allocated array size   */
    static idx_t  max_computed = 2;  /* highest index already calculated */

    /* Allocate or grow the buffer only on first request for n >= 3. */
    if (cache == NULL) {
        /* Reserve space for indices 0 … n.  Minimum size 3 for the
         * initial values. */
        capacity = (n + 1) > 3 ? (n + 1) : 3;
        cache = (seq_t *)malloc((size_t)capacity * sizeof(seq_t));
        if (!cache) return (seq_t)0;           /* allocation failure */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)6;
        cache[2] = (seq_t)72;
    } else if (n + 1 > capacity) {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;           /* allocation failure */
        cache = tmp;
        capacity = new_cap;
    }

    /* If the value is already computed, return it immediately. */
    if (n <= max_computed)
        return cache[n];

    /* Compute any missing terms up to n.  Use helper mul_small to
     * prevent overflow when seq_t is small.  The addition is done
     * in the native type of seq_t. */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t term = mul_small(cache[i - 1], 6) + mul_small(cache[i - 2], 42);
        cache[i] = term;
    }
    max_computed = n;
    return cache[n];
}
