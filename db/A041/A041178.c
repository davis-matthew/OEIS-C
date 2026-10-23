/*
 * A041178  Numerators of continued fraction convergents to sqrt(99).
 * https://oeis.org/A041178
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 13, u64 n <= 29, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A041178
   Numerators of continued fraction convergents to sqrt(99).
   Recurrence:
       a(0) = 9
       a(1) = 10
       a(2) = 189
       a(3) = 199
       a(n) = 20·a(n-2) – a(n-4)   for n ≥ 4
   The sequence is memoised; calls may arrive in any order. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cap   = 0;   /* number of elements allocated */

    if (n >= cap)
    {
        idx_t old_cap = cap;
        idx_t new_cap = n + 1;   /* need indices 0 … n */

        /* realloc works with void *, cast to the proper pointer type in C++ */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;      /* allocation failure – not expected */
        cache = tmp;
        cap   = new_cap;

        /* Initialise the first four terms when we allocate or reallocate. */
        if (cap > 0) cache[0] = (seq_t)9;
        if (cap > 1) cache[1] = (seq_t)10;
        if (cap > 2) cache[2] = (seq_t)189;
        if (cap > 3) cache[3] = (seq_t)199;

        /* Compute all terms from the first missing index up to n. */
        for (idx_t i = old_cap; i <= n; ++i)
        {
            if (i < 4) continue;          /* already initialised */
            cache[i] = (seq_t)20 * cache[i-2] - cache[i-4];
        }
    }

    /* Cache is now large enough to contain a(n). */
    return cache[n];
}
