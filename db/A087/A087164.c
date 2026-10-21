/*
 * A087164  Records in A087162: A087162(a(n))=n.
 * https://oeis.org/A087164
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A087164 (records in A087162).
   Recurrence: a(1)=1, a(2)=2, a(3)=3, a(4)=7, a(5)=23.
   For n≥6:  a(n) = 6*a(n-1) - 8*a(n-2) + 3*a(n-3).
   Offset 1 (a(1) is the first term). */
#include <stdlib.h>

#if defined(__cplusplus)
# define ALLOC_SEQ_T(ptr,size) ((ptr) = reinterpret_cast<seq_t*>(calloc((size), sizeof(seq_t))))
# define REALLOC_SEQ_T(ptr,newsize) ((ptr) = reinterpret_cast<seq_t*>(realloc((ptr), (newsize) * sizeof(seq_t))))
#else
# define ALLOC_SEQ_T(ptr,size) ((ptr) = (seq_t*)calloc((size), sizeof(seq_t)))
# define REALLOC_SEQ_T(ptr,newsize) ((ptr) = (seq_t*)realloc((ptr), (newsize) * sizeof(seq_t)))
#endif

seq_t a(idx_t n)
{
    static seq_t *values = NULL;      /* cache of computed terms (index 1..max_n) */
    static idx_t  max_n   = 0;        /* highest index computed so far    */
    static size_t cap     = 0;        /* allocated array size (size_t)   */

    /* First call – allocate an initial buffer and seed initial values. */
    if (!values) {
        cap = 16;                     /* arbitrary initial capacity */
        ALLOC_SEQ_T(values, cap);     /* zero‑initialised */
        if (!values) abort();         /* allocation failed */

        values[1] = 1;
        values[2] = 2;
        values[3] = 3;
        values[4] = 7;
        values[5] = 23;
        max_n = 5;
    }

    /* If the requested index is already cached, return it. */
    if (n <= max_n)
        return values[n];

    /* Ensure the allocated array is large enough for indices up to n. */
    if (cap < (size_t)(n + 1)) {
        size_t new_cap = cap * 2;
        if (new_cap < (size_t)(n + 1))
            new_cap = n + 1;
        REALLOC_SEQ_T(values, new_cap);
        if (!values) abort();
        cap = new_cap;
    }

    /* Compute all missing terms using the recurrence. */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        /* Applying the recurrence for i≥6.  For i=6, the formula
           uses values[5], values[4], values[3] which are already
           initialized. */
        values[i] = 6 * values[i - 1] - 8 * values[i - 2] + 3 * values[i - 3];
    }
    max_n = n;
    return values[n];
}
