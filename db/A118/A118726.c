/*
 * A118726  a(n) = Sum_{k=0..n} F(n+k)*binomial(n+k,k) where F=A000045.
 * https://oeis.org/A118726
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..n} F(n+k) * C(n+k,k), F is Fibonacci (F0=0,F1=1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Cache for Fibonacci numbers up to the largest requested index */
static seq_t *fib_cache = NULL;
static size_t fib_cap = 0;

/* Ensure fib_cache contains F[0..max_n]   (max_n is idx_t) */
static void ensure_fib_capacity(idx_t max_n)
{
    if (fib_cap > (size_t)max_n)
        return;

    size_t new_cap = fib_cap > 0 ? fib_cap : 2;
    while (new_cap <= (size_t)max_n)
        new_cap <<= 1;

    seq_t *new_ptr = (seq_t *)realloc(fib_cache, new_cap * sizeof(seq_t));
    if (!new_ptr)    /* allocation failed – return zero on demand */
        return;

    /* initialise new entries */
    if (fib_cap == 0) {
        new_ptr[0] = 0;
        if (new_cap > 1)
            new_ptr[1] = 1;
        size_t i;
        for (i = 2; i < new_cap; ++i)
            new_ptr[i] = new_ptr[i - 1] + new_ptr[i - 2];
    } else {
        size_t i;
        for (i = fib_cap; i < new_cap; ++i)
            new_ptr[i] = new_ptr[i - 1] + new_ptr[i - 2];
    }

    fib_cache = new_ptr;
    fib_cap   = new_cap;
}

/* -----  Main sequence function  ----- */
seq_t
a(idx_t n)
{
    /* need Fibonacci numbers up to 2*n */
    ensure_fib_capacity(n + n);

    seq_t sum   = 0;           /* current sum */
    seq_t binom = 1;           /* C(n+0,0) */

    for (idx_t k = 0; k <= n; ++k) {
        size_t idx = (size_t)(n + k);        /* index into fib_cache */
        seq_t term = fib_cache[idx] * binom; /* F(n+k) * C(n+k,k) */
        sum += term;

        if (k == n)        /* final iteration – no further binomial needed */
            break;

        /* compute binom = binom * (n+k+1) / (k+1) */
        binom = binom * (seq_t)(n + k + 1);
        binom = binom / (seq_t)(k + 1);
    }

    return sum;
}
