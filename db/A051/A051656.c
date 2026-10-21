/*
 * A051656  a(n) = Sum_{i=0..n} (C(n,i) mod 2)*Fibonacci(2*i).
 * https://oeis.org/A051656
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = Σ_{i=0..n} (C(n,i) mod 2) * Fibonacci(2*i)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 46, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cached Fibonacci numbers: fib[k] = F(k) */
static seq_t *fib = NULL;      /* pointer to array of Fibonacci values   */
static size_t fib_len = 0;     /* number of Fibonacci values currently stored */
static size_t fib_cap = 0;     /* capacity of the allocated array         */

/* Ensure that fib[0..up_to] are computed and available.  */
static void ensure_fib(size_t up_to)
{
    if (!fib) {
        fib_cap = (up_to < 4 ? 4 : up_to + 1);
        fib = (seq_t*)malloc(fib_cap * sizeof(seq_t));
        if (!fib) return;                      /* allocation failed → nothing to do */
        fib[0] = 0;
        fib[1] = 1;
        fib_len = 2;
    }

    /* Grow the array if necessary */
    if (fib_cap <= up_to) {
        size_t new_cap = fib_cap;
        while (new_cap <= up_to) new_cap *= 2;
        seq_t *tmp = (seq_t*)realloc(fib, new_cap * sizeof(seq_t));
        if (!tmp) return;                      /* allocation failed → nothing to do */
        fib = tmp;
        fib_cap = new_cap;
    }

    /* Append new Fibonacci numbers up to up_to */
    while (fib_len <= up_to) {
        fib[fib_len] = fib[fib_len - 1] + fib[fib_len - 2];
        ++fib_len;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Always use seq_t for intermediate values. */
    /* Fibonacci indices needed: 0 .. 2*n. */
    size_t upto = (size_t)n * 2;
    ensure_fib(upto);

    seq_t result = 0;
    /* Sum over i with parity condition: (i & ~n) == 0  */
    for (idx_t i = 0; i <= n; ++i) {
        if ((i & (~n)) == 0) {
            result += fib[2 * i];
        }
    }
    return result;
}
