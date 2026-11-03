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
 * Validated: partial, pass, 62 terms through n = 62 (2026-11-03)
 * Helpers:   gcd, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(1) = 1
   For n > 1, a(n) = smallest integer > a(n-1) that is coprime to Fibonacci(n)
   (where Fibonacci(1) = 1, Fibonacci(2) = 1, …) */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/mem.h"

static seq_t fib(idx_t n) {
    if (n <= 2) return (seq_t)1;
    seq_t f1 = 1, f2 = 1, f3;
    for (idx_t i = 3; i <= n; ++i) {
        f3 = f1 + f2;
        f1 = f2;
        f2 = f3;
    }
    return f2;
}

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cap   = 0; /* allocated capacity (in terms of n) */
    static size_t len   = 0; /* number of terms currently stored (1‑based) */

    /* Grow the cache so it can hold indices up to n */
    if (n > cap) {
        size_t newcap = (cap > 0) ? cap * 2 : 1;
        while (newcap < n) newcap *= 2;
        seq_t *tmp = mem_resize(seq_t, cache, newcap);
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
