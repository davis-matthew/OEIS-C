/*
 * A076999  a(1) = 1, a(n+1) is the largest Fibonacci number <= n*a(n).
 * https://oeis.org/A076999
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 14, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1,
   a(n+1) = largest Fibonacci number <= n * a(n)   (n >= 1) */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* stored terms a(1)...a(len)          */
    static size_t cap   = 0;      /* capacity of cache                  */
    static size_t len   = 0;      /* current number of stored terms      */

    /* Fibonacci numbers are maintained in static variables.  They are
       reset only when the cache grows large enough that a new Fibonacci
       number might be needed for the next term.  */
    static seq_t fib_prev = 1;    /* F1  */
    static seq_t fib_curr = 1;    /* F2  */

    if (n == 0) {                 /* undefined, just rescue */
        return 0;
    }

    /* Allocate or grow the cache to fit n entries if necessary.  The
       cast is required for C++.  It is safe because all elements are
       later assigned explicitly. */
    if (n > cap) {
        size_t new_cap = cap ? cap << 1 : 16;
        while (new_cap < n) {
            new_cap <<= 1;
        }
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        cap   = new_cap;
    }

    /* Ensure a(1) is present.  Other terms are computed lazily and only
       when needed. */
    if (len == 0) {
        cache[0] = 1;
        len      = 1;
    }

    /* Compute terms up to the requested index.  Each new term is the
       largest Fibonacci number not exceeding (k-1) * a(k-1). */
    for (idx_t k = len + 1; k <= n; ++k) {
        seq_t target = ((seq_t)(k - 1)) * cache[k - 2];

        /* Advance the Fibonacci pair until fib_curr > target.  Then
           fib_prev is the largest Fibonacci <= target. */
        while (fib_curr <= target) {
            seq_t next = fib_prev + fib_curr;
            fib_prev = fib_curr;
            fib_curr = next;
        }
        cache[k - 1] = fib_prev;
    }
    len = n;
    return cache[n - 1];
}
