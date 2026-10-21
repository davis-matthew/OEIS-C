/*
 * A232165  Cardinality of the Weyl alternation set corresponding to the
 *          zero-weight in the adjoint representation of the Lie algebra
 *          sp(2n).
 * https://oeis.org/A232165
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 29, u64 n <= 58, u128 n <= 114,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A232165
 * Recurrence: a(n) = a(n-1) + a(n-2) + 3*a(n-3) + a(n-4)
 * Initial values: a(0)=0, a(1)=1, a(2)=2, a(3)=3
 * The function memoises computed values so that it runs in O(n) time
 * and uses O(n) space. It works for any build where seq_t is an
 * unsigned integer type, an arbitrary‑precision integer class, or
 * a POD struct with arithmetic operators.
 */

#include <stdlib.h> /* malloc, realloc */

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;   /* pointer to array of values      */
    static size_t capacity = 0;   /* size of allocated array         */
    static idx_t last_computed = 0;   /* highest index already computed */

    /* Ensure the allocated array is large enough for the requested
       index and for the four initial values when the cache is first
       created.  */
    if (capacity <= (size_t)n || capacity < 4) {
        size_t new_cap = capacity ? capacity * 2 : 4;
        if (new_cap <= (size_t)n) new_cap = n + 1;
        if (new_cap < 4) new_cap = 4;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        capacity = new_cap;
    }

    /* Initialise the first four entries (only once). */
    if (last_computed < 3) {
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)3;
        last_computed = 3;
    }

    /* Return the value if we have already computed it. */
    if (n <= (idx_t)last_computed) {
        return cache[(size_t)n];
    }

    /* Compute remaining terms up to n. */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t v = cache[(size_t)i-1] + cache[(size_t)i-2]
                + (seq_t)3 * cache[(size_t)i-3]
                + cache[(size_t)i-4];
        cache[(size_t)i] = v;
    }
    last_computed = n;

    return cache[(size_t)n];
}
