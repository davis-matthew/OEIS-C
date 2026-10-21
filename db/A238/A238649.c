/*
 * A238649  Number of (n+2) X (3+2) 0..1 arrays with no element equal to all
 *          horizontal neighbors or equal to all vertical neighbors, and new
 *          values 0..1 introduced in row major order.
 * https://oeis.org/A238649
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 35, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 2*a(n-1) + 5*a(n-2) - a(n-4),
   with a(1)=3, a(2)=18, a(3)=51, a(4)=189.  Offset = 1. */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

/* External linkage for the sequence function */
seq_t a(idx_t n)
{
    /* Static cache of computed terms, index 0 unused (offset 1) */
    static seq_t *cache = NULL;
    static size_t capacity = 0;      /* size of allocated array */
    static idx_t max_computed = 0;   /* highest index that has been filled */

    if (n < 1) return 0;            /* undefined for n<1 */

    /* Ensure the cache is large enough for indices 0 .. n */
    if (capacity <= (size_t)n) {
        size_t new_cap = (size_t)n + 1;   /* include index n */
        seq_t *new_arr = NULL;

#ifdef __cplusplus
        /* Allocate objects with new[] so that constructors run for
           arbitrary-precision types.  new[] also guarantees proper
           alignment. */
        new_arr = new seq_t[new_cap];
#else
        /* Allocate raw storage for POD types */
        new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_arr) exit(EXIT_FAILURE);
#endif
        /* Copy existing values to the new array */
        for (size_t i = 0; i < capacity; ++i) {
            new_arr[i] = cache[i];
        }
        /* Initialize new elements to zero (constructors already called
           for C++ builds). */
        for (size_t i = capacity; i < new_cap; ++i) {
            new_arr[i] = (seq_t)0;
        }

        /* Free old storage */
        if (cache != NULL) {
#ifdef __cplusplus
            delete[] cache;
#else
            free(cache);
#endif
        }
        cache = new_arr;
        capacity = new_cap;
    }

    /* Initialize the first few terms if the array is large enough.
       Writing the same values multiple times is harmless because they
       are identical. */
    if (capacity > 1) cache[1] = (seq_t)3;
    if (capacity > 2) cache[2] = (seq_t)18;
    if (capacity > 3) cache[3] = (seq_t)51;
    if (capacity > 4) cache[4] = (seq_t)189;

    /* Compute missing terms using the recurrence. */
    idx_t start = max_computed + 1;
    if (start < 5) start = 5;   /* recurrence requires a(n-4)   */
    for (idx_t i = start; i <= n; ++i) {
        size_t idx = (size_t)i;
        seq_t term = cache[idx - 1] * (seq_t)2
                    + cache[idx - 2] * (seq_t)5
                    - cache[idx - 4];
        cache[idx] = term;
    }
    if (n > max_computed) max_computed = n;

    return cache[(size_t)n];
}
