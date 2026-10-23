/*
 * A023613  Convolution of Fibonacci numbers and A023533.
 * https://oeis.org/A023613
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n+1} F(k) * A023533(n+2-k) where F(k) is the
 *            k‑th Fibonacci number (F(1)=F(2)=1) and A023533(m) = 1 if m is a
 *            tetrahedral number t = m(m+1)(m+2)/6, 0 otherwise
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 46, u64 n <= 92, u128 n <= 184,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* cached Fibonacci numbers, 1‑based indexing */
static seq_t *fib_cache = NULL;   /* array of Fibonacci numbers */
static size_t fib_cap = 0;       /* allocated capacity (size of fib_cache) */
static idx_t  fib_last = 0;      /* highest index that has been computed */

/* ensure Fibonacci numbers up to index n+1 are available */
static void ensure_fib(idx_t n)
{
    idx_t needed = n + 1;                    /* largest Fibonacci index required */
    if (fib_cache == NULL) {                 /* first allocation */
        size_t init_cap = needed + 2;         /* some slack for growth */
        fib_cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        fib_cap = init_cap;
        fib_cache[1] = 1;
        fib_cache[2] = 1;
        fib_last = 2;
    }
    if (fib_last < needed) {                 /* grow and compute new terms */
        if (fib_cap <= needed) {
            size_t new_cap = needed * 2 + 2;  /* exponential growth for speed */
            fib_cache = (seq_t *)realloc(fib_cache, new_cap * sizeof(seq_t));
            fib_cap = new_cap;
        }
        for (idx_t i = fib_last + 1; i <= needed; ++i) {
            fib_cache[i] = fib_cache[i-1] + fib_cache[i-2];
        }
        fib_last = needed;
    }
}

/* compute a(n) according to the convolution definition */
seq_t a(idx_t n)
{
    ensure_fib(n);                           /* need F(1)…F(n+1) */

    seq_t sum = 0;
    idx_t limit = n + 1;                     /* t must not exceed this */

    for (idx_t m = 1; ; ++m) {
        idx_t t = m * (m + 1) * (m + 2) / 6; /* tetrahedral number */
        if (t > limit) break;
        idx_t k = n + 2 - t;                 /* index into Fibonacci array (1‑based) */
        sum += fib_cache[k];
    }
    return sum;
}
