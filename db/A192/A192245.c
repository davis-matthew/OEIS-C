/*
 * A192245  1-sequence of reduction of triangular number sequence by x^2 ->
 *          x+1.
 * https://oeis.org/A192245
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n-1} A000217(k+1)*Fibonacci(k)
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 33, u64 n <= 76, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Static state for memoisation */
static size_t a_cache_size = 0;
static seq_t *a_cache = NULL;
static size_t a_cache_cap = 0;
static seq_t fib_i_minus1; /* Fibonacci(0) */
static seq_t fib_i;        /* Fibonacci(1) */

/* Ensure the cache has room for at least `needed` elements. */
static void a_ensure_capacity(size_t needed)
{
    if (needed <= a_cache_cap)
        return;
    size_t new_cap = a_cache_cap ? a_cache_cap << 1 : 8;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    a_cache = tmp;
    a_cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0; /* undefined, but safe */

    if (a_cache_size == 0) {
        /* initialise first term a(1) = 0 */
        a_ensure_capacity(1);
        a_cache[0] = (seq_t)0;
        a_cache_size = 1;
        fib_i_minus1 = (seq_t)0; /* Fib(0) */
        fib_i = (seq_t)1;        /* Fib(1) */
    }

    while (a_cache_size < n) {
        idx_t k = a_cache_size; /* k is the current 1‑based index */

        /* Triangular number T_{k+1} = (k+1)(k+2)/2 */
        seq_t triangular = ((seq_t)(k + 1) * (seq_t)(k + 2)) / (seq_t)2;

        /* Term to add: Fib(k) * T_{k+1} */
        seq_t add = fib_i * triangular;

        a_ensure_capacity(a_cache_size + 1);
        a_cache[a_cache_size] = a_cache[a_cache_size - 1] + add;

        /* Advance Fibonacci numbers: Fib(k+1) = Fib(k-1) + Fib(k) */
        seq_t fib_next = fib_i_minus1 + fib_i;
        fib_i_minus1 = fib_i;
        fib_i = fib_next;

        a_cache_size++;
    }

    return a_cache[n - 1];
}
