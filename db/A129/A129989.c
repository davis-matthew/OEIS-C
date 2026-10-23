/*
 * A129989  a(1)=1. a(n) = smallest integer > a(n-1) that is coprime to the
 *          n-th Fibonacci number.
 * https://oeis.org/A129989
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 25, u32 n <= 48, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   fib, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(1) = 1
   For n > 1, a(n) = smallest integer > a(n-1) that is coprime to Fibonacci(n)
   (where Fibonacci(1) = 1, Fibonacci(2) = 1, …) */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/fib.h"
#include "../utils/gcd.h"

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cap   = 0; /* allocated capacity (in terms of n) */
    static size_t len   = 0; /* number of terms currently stored (1‑based) */

    /* Grow the cache so it can hold indices up to n */
    if (n > cap) {
        size_t newcap = (cap > 0) ? cap * 2 : 1;
        while (newcap < n) newcap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp && newcap > 0) abort(); /* allocation failed */
        cache = tmp;
        cap   = newcap;
    }

    /* Compute all missing terms up to n (in order), in case the calls are out of order */
    for (size_t k = len + 1; k <= n; ++k) {
        if (k == 1) {                /* base case */
            cache[0] = (seq_t)1;
            continue;
        }

        seq_t prev = cache[k - 2];    /* a(k-1) */
        seq_t fk   = fib(k);         /* Fibonacci(k) */
        seq_t cand = prev + (seq_t)1;

        while (gcd(cand, fk) != (seq_t)1)
            cand += (seq_t)1;

        cache[k - 1] = cand;          /* store a(k) */
    }

    len = n;
    return cache[n - 1];
}
