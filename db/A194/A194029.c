/*
 * A194029  Natural fractal sequence of the Fibonacci sequence (1, 2, 3, 5, 8,
 *          ...).
 * https://oeis.org/A194029
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = 1 + n - F_k where F_k is the largest Fibonacci number ≤
 *            n. The sequence is the natural fractal sequence of the Fibonacci
 *            numbers
 * Range:     u8 n <= 864, u16 n <= 9047, u32 n <= 9047, u64 n <= 9047,
 *            u128 n <= 9047, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>


/* static cache of Fibonacci numbers 1, 2, 3, 5, 8, … */
static idx_t *fib_cache = NULL;
static size_t fib_len  = 0;   /* number of stored Fibonacci values */
static size_t fib_cap  = 0;   /* capacity of the cache */

/* Ensure fib_cache contains Fibonacci numbers up to at least n. */
static void ensure_fib(idx_t n)
{
    /* initialise the cache once */
    if (fib_len == 0)
    {
        fib_cap = 8;
        fib_cache = (idx_t *)malloc(fib_cap * sizeof(idx_t));
        fib_cache[0] = 1;      /* F(1) */
        fib_cache[1] = 2;      /* F(2) */
        fib_len   = 2;
    }

    /* grow until fib_cache[fib_len-1] > n */
    while (n >= fib_cache[fib_len - 1])
    {
        if (fib_len == fib_cap)
        {
            fib_cap <<= 1;                     /* double capacity */
            fib_cache = (idx_t *)realloc(fib_cache,
                                      fib_cap * sizeof(idx_t));
        }
        fib_cache[fib_len] = fib_cache[fib_len - 1]
                             + fib_cache[fib_len - 2];
        fib_len++;
    }
}

seq_t a(idx_t n)
{
    /* Prepare cache for this n */
    ensure_fib(n);

    /* binary search for largest Fibonacci ≤ n */
    size_t low  = 0;
    size_t high = fib_len;   /* exclusive */

    while (low + 1 < high)
    {
        size_t mid = (low + high) >> 1;
        if (fib_cache[mid] <= n)
            low = mid;
        else
            high = mid;
    }

    /* Compute a(n) = 1 + n - Fibonacci */
    idx_t off = n - fib_cache[low];
    return (seq_t)(off + 1);
}
