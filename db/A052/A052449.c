/*
 * A052449  a(n) = 1 + Product_{k=1..n} Fibonacci(k).
 * https://oeis.org/A052449
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 10, u64 n <= 14, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 1 + ∏_{k=1..n} F_k
 * where F_k is the k‑th Fibonacci number (F_1 = 1, F_2 = 1).
 *
 * The implementation caches Fibonacci numbers and the cumulative
 * product so that repeated calls in any order are handled in O(n)
 * time per request, with O(n) extra space.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the offset (a(1) is the first term). */
    if (n == 0)
        return (seq_t)0;

    /* Static state to preserve results between calls. */
    static idx_t max_n = 0;            /* largest n for which arrays are valid   */
    static seq_t *prod = NULL;         /* prod[i] = ∏_{k=1..i} F_k   (prod[0] = 1) */
    static seq_t *fib  = NULL;         /* fib[i] = F_i                      */
    static idx_t prod_cap = 0;         /* current capacity of prod array      */
    static idx_t fib_cap  = 0;         /* current capacity of fib array       */
    idx_t i, new_cap;

    /* Ensure at least space for indices 0..n (1‑based for fib). */
    new_cap = (max_n == 0) ? 3 : n + 1;
    if (new_cap > prod_cap)
    {
        prod_cap = new_cap;
        prod = (seq_t*)realloc(prod, prod_cap * sizeof(seq_t));
    }
    if (new_cap > fib_cap)
    {
        fib_cap = new_cap;
        fib = (seq_t*)realloc(fib, fib_cap * sizeof(seq_t));
    }

    /* Initialisation: set base Fibonacci values and their product. */
    if (max_n == 0)
    {
        /* Fibonacci: F_1 = 1, F_2 = 1 */
        fib[1] = (seq_t)1;
        fib[2] = (seq_t)1;

        /* Cumulative product: prod[0] = 1; prod[1] = F_1; prod[2] = F_1*F_2 */
        prod[0] = (seq_t)1;
        prod[1] = prod[0] * fib[1];
        prod[2] = prod[1] * fib[2];
        max_n = 2;
    }

    /* Extend the tables up to the requested n. */
    for (i = max_n + 1; i <= n; ++i)
    {
        /* Compute next Fibonacci number */
        fib[i] = fib[i - 1] + fib[i - 2];
        /* Update cumulative product */
        prod[i] = prod[i - 1] * fib[i];
    }
    if (i - 1 > max_n)
        max_n = i - 1;

    /* Return the sequence value a(n) = 1 + product of the first n Fibonacci numbers. */
    return prod[n] + (seq_t)1;
}
