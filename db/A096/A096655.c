/*
 * A096655  a(n) = F(n+1)*a(n-1) + F(n)*a(n-2), where F = A000045 (Fibonacci
 *          numbers), a(0)=1, a(1)=1.
 * https://oeis.org/A096655
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = F(n+1)*a(n-1) + F(n)*a(n-2), a(0) = 1, a(1) = 1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for computed terms and Fibonacci helpers */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t fib0 = 0, fib1 = 0;   /* fib0 = F(k), fib1 = F(k+1) for last index k = cache_len-1 */

    /* Initialisation */
    if (cache == NULL) {
        cache_cap = 2;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(1);
        cache[0] = 1;            /* a(0) */
        cache[1] = 1;            /* a(1) */
        cache_len = 2;
        fib0 = 1;                /* F(1) */
        fib1 = 1;                /* F(2) */
    }

    /* If the value is already in the cache, return it */
    if (n < cache_len) return cache[n];

    /* Extend the cache up to index n */
    while (cache_len <= n) {
        /* Ensure enough capacity */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) exit(1);
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Compute next Fibonacci numbers */
        seq_t fib2 = fib0 + fib1; /* F(k+2) where k = cache_len-1 */

        /* Compute a(k+1) = F(k+2)*a(k) + F(k+1)*a(k-1) */
        seq_t new_a = fib2 * cache[cache_len - 1] + fib1 * cache[cache_len - 2];

        /* Store the new term */
        cache[cache_len] = new_a;
        cache_len++;

        /* Shift Fibonacci helpers for next iteration */
        fib0 = fib1;
        fib1 = fib2;
    }

    return cache[n];
}
