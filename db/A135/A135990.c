/*
 * A135990  Expansion of x^3*(x-1)*(x+1) / (x^5-2*x^4+x^2-1).
 * https://oeis.org/A135990
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, less
 * Formula:   not stated
 * Range:     i8 n <= 27, i16 n <= 48, i32 n <= 57, i64 n <= 57, i128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A135990: a(n) = coefficient of x^n in
   x^3*(x-1)*(x+1) / (x^5-2*x^4+x^2-1).
   Recurrence (order 5, offset 1):
      a(1)=0, a(2)=0, a(3)=1, a(4)=0, a(5)=0,
      a(n)=a(n-2)-2*a(n-4)+a(n-5)   for n≥6. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* The sequence is indexed from 1. For n<1 return 0. */
    if (n < 1)
        return (seq_t)0;

    static seq_t *cache = NULL;
    static idx_t  max_n = 0;
    static size_t cap    = 0;

    size_t target = (size_t)n;

    /* Grow the cache if necessary.  The cast ensures compatibility in C++ */
    if (target >= cap)
    {
        size_t new_cap = target + 1;          /* one extra slot for 0‑index */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;                  /* allocation failure */
        cache = tmp;
        /* Initialise new entries to zero (safe for simple types and trivial
           classes).  They will be overwritten by the recurrence. */
        for (size_t i = cap; i < new_cap; ++i)
            cache[i] = (seq_t)0;
        cap = new_cap;
    }

    /* Compute all missing terms up to `target`. */
    if (target > max_n)
    {
        for (size_t i = max_n + 1; i <= target; ++i)
        {
            if (i <= 5)               /* initial values */
            {
                switch((idx_t)i)
                {
                    case 1: cache[i] = (seq_t)0; break;
                    case 2: cache[i] = (seq_t)0; break;
                    case 3: cache[i] = (seq_t)1; break;
                    case 4: cache[i] = (seq_t)0; break;
                    case 5: cache[i] = (seq_t)0; break;
                }
            }
            else                       /* recurrence for i≥6 */
            {
                cache[i] = cache[i-2] - (seq_t)2 * cache[i-4] + cache[i-5];
            }
        }
        max_n = target;
    }

    return cache[target];
}
