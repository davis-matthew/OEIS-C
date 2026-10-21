/*
 * A099428  Duplicate of A099920.
 * https://oeis.org/A099428
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = n * Fib(n-1) where Fib(0)=0, Fib(1)=1 and
 *            Fib(k)=Fib(k-1)+Fib(k-2)
 * Range:     i8 n <= 8, i16 n <= 17, i32 n <= 17, i64 n <= 17, i128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle trivial cases directly. */
    if (n < 2)
        return 0;

    static seq_t *fib   = NULL;   /* Fibonacci cache: fib[0] = 0, fib[1] = 1. */
    static idx_t  cap   = 0;      /* Current allocated capacity of fib[]. */
    static idx_t  last  = 1;      /* Highest index already computed. */

    idx_t idx = n - 1;            /* We need Fib(n-1). */

    /* Initial allocation for the cache. */
    if (fib == NULL) {
        cap  = 2;
        fib  = (seq_t *)calloc(cap, sizeof(seq_t));
        if (fib == NULL) abort();   /* Allocation failure should never happen. */
        fib[0] = 0;
        fib[1] = 1;
        last   = 1;
    }

    /* Grow the cache if needed. */
    if (cap <= idx) {
        /* Double the capacity until it can hold idx. */
        idx_t new_cap = cap ? cap : 1;
        while (new_cap <= idx)
            new_cap <<= 1;
        fib = (seq_t *)realloc(fib, new_cap * sizeof(seq_t));
        if (fib == NULL) abort();
        cap = new_cap;
    }

    /* Compute any missing Fibonacci numbers up to idx. */
    if (idx > last) {
        for (idx_t i = last + 1; i <= idx; ++i)
            fib[i] = fib[i - 1] + fib[i - 2];
        last = idx;
    }

    /* a(n) = n * Fib(n-1) */
    return (seq_t)n * fib[idx];
}
