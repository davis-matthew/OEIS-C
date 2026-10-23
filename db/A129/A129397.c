/*
 * A129397  Row sums of A129396.
 * https://oeis.org/A129397
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -4*a(n-1) - 6*a(n-2) + 4*a(n-3) + 15*a(n-4) + 8*a(n-5) -
 *            16*a(n-6) with a(0)=1, a(1)=9, a(2)=-29, a(3)=61, a(4)=-17,
 *            a(5)=-263
 * Range:     i8 n <= 4, i16 n <= 10, i32 n <= 26, i64 n <= 31, i128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for realloc, abort */

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;  /* dynamic memoization buffer   */
    static size_t cap   = 0;     /* allocated size                */
    static size_t known = 0;     /* indices [0 .. known-1] have values */

    /* The first six values, needed for the recurrence */
    static const seq_t init[6] = {1, 9, -29, 61, -17, -263};

    /* If we already computed this term, return it directly. */
    if (cache && (size_t)n < known)
        return cache[(size_t)n];

    /* For n<6 the value is known immediately.  */
    if (n < 6)
        return init[(size_t)n];

    /* Allocate more space if required.  Double the size each time.  */
    if ((size_t)n >= cap) {
        size_t new_cap = cap ? cap : 6;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();                 /* allocation failed */
        cache = tmp;
        cap   = new_cap;

        /* Initialise the first six entries.  */
        for (size_t i = 0; i < 6 && i < cap; ++i)
            cache[i] = init[i];
        if (known < 6)
            known = 6;
    }

    /* Compute missing terms up to n using the recurrence.  */
    for (size_t i = known; i <= (size_t)n; ++i) {
        cache[i] = -cache[i-1] * 4
                 - cache[i-2] * 6
                 + cache[i-3] * 4
                 + cache[i-4] * 15
                 + cache[i-5] * 8
                 - cache[i-6] * 16;
    }
    known = (size_t)n + 1;

    return cache[(size_t)n];
}
