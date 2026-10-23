/*
 * A130234  Minimal index k of a Fibonacci number such that Fibonacci(k) >= n
 *          (the 'upper' Fibonacci Inverse).
 * https://oeis.org/A130234
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = minimal k with Fibonacci(k) ≥ n (F_0 = 0, F_1 = 1)
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Special case: n = 0 */
    if (n == 0)
        return (seq_t)0;

    /* Static cache of Fibonacci numbers.  We grow this lazily. */
    static seq_t *fib_cache = NULL;
    static idx_t fib_len   = 0;   /* number of cached Fibonacci numbers */
    static idx_t fib_cap   = 0;   /* allocated capacity */

    /* One-time initialization */
    if (fib_cache == NULL) {
        fib_cap = 4;                               /* initial capacity */
        fib_cache = (seq_t *)malloc(fib_cap * sizeof(seq_t));
        if (!fib_cache) return (seq_t)0;           /* allocation failure fallback */
        fib_cache[0] = (seq_t)0;                   /* F_0 */
        fib_cache[1] = (seq_t)1;                   /* F_1 */
        fib_len = 2;
    }

    /* Extend the cache until fib_cache[fib_len-1] >= n */
    seq_t n_seq = (seq_t)n;   /* cast once for comparison */
    while (fib_cache[fib_len - 1] < n_seq) {
        /* Ensure capacity */
        if (fib_len >= fib_cap) {
            idx_t new_cap = fib_cap << 1;          /* double */
            seq_t *tmp = (seq_t *)realloc(fib_cache, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;            /* allocation failure fallback */
            fib_cache = tmp;
            fib_cap = new_cap;
        }
        /* Compute next Fibonacci number */
        fib_cache[fib_len] = fib_cache[fib_len - 1] + fib_cache[fib_len - 2];
        fib_len++;
    }

    /* Binary search for first index with fib_cache[idx] >= n_seq */
    idx_t lo = 0, hi = fib_len - 1;
    while (lo < hi) {
        idx_t mid = (lo + hi) >> 1;
        if (fib_cache[mid] >= n_seq)
            hi = mid;
        else
            lo = mid + 1;
    }
    return (seq_t)lo;
}
