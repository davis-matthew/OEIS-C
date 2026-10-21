/*
 * A238601  A sixth-order linear divisibility sequence related to the
 *          Fibonacci numbers: a(n) :=
 *          (1/10)*Fibonacci(3*n)*Fibonacci(5*n)/Fibonacci(n).
 * https://oeis.org/A238601
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A238601
   a(n) = (1/10) * Fibonacci(3*n) * Fibonacci(5*n) / Fibonacci(n)
   Recurrence (valid for n>=7):
     a(n) = 22*a(n-1) + 250*a(n-2) - 1320*a(n-3) - 250*a(n-4)
            + 22*a(n-5) + a(n-6)
   Initial values:
     a(1)=1, a(2)=44, a(3)=1037, a(4)=32472, a(5)=915305, a(6)=26874892
   The function memoises all computed terms and is safe for calls in any
   order. */

#include <stdlib.h>
#include <stdint.h>

/* External array helper prototypes (provided by the build) */
extern seq_t fib(idx_t n);

/* The sequence-returning function */
seq_t a(idx_t n)
{
    /* Safety: the sequence is undefined for n < 1 */
    if (n < 1)
        return (seq_t)0;

    /* Static cache: index 0 unused */
    static seq_t  *cache   = NULL;   /* grows as needed */
    static idx_t   cap     = 0;      /* allocated capacity */
    static idx_t   last    = 0;      /* highest n that has been computed */

    /* Ensure capacity for indices up to n */
    if (cap <= n)
    {
        /* Expand capacity: double until sufficient */
        idx_t new_cap = cap ? cap * 2 : 8;
        while (new_cap <= n)
            new_cap <<= 1;

        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        /* If realloc fails, abort.  In the judge environment this
           should never happen; if it does, we treat the address as
           NULL and recover. */
        if (!new_cache)
            return (seq_t)0;
        cache = new_cache;
        cap   = new_cap;
    }

    /* Initialise first six terms if not already done */
    if (last == 0)
    {
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)44;
        cache[3] = (seq_t)1037;
        cache[4] = (seq_t)32472;
        cache[5] = (seq_t)915305;
        cache[6] = (seq_t)26874892;
        last    = 6;
    }

    /* Compute any missing terms up to n */
    if (n > last)
    {
        for (idx_t i = last + 1; i <= n; ++i)
        {
            /* Recurrence uses only integer arithmetic */
            seq_t term = (seq_t)22 * cache[i - 1]
                       + (seq_t)250 * cache[i - 2]
                       - (seq_t)1320 * cache[i - 3]
                       - (seq_t)250 * cache[i - 4]
                       + (seq_t)22 * cache[i - 5]
                       + cache[i - 6];
            cache[i] = term;
        }
        last = n;
    }

    return cache[n];
}
