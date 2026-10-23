/*
 * A177727  a(0)=1; a(n) = a(n-1) * Fibonacci(3+n) * Fibonacci(1+n) /
 *          (Fibonacci(n))^2, n > 1.
 * https://oeis.org/A177727
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1; a(1)=3; for n>1:
 *            a(n)=a(n-1)*Fib(n+3)*Fib(n+1)/(Fib(n))^2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 23, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Requires a Fibonacci function for arbitrary‑precision ints.
   We compute Fibonacci numbers on the fly with a static cache. */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* ------------------ Fibonacci cache ------------------ */
    static seq_t *fib_cache = NULL;    /* fib_cache[i] = Fib(i) */
    static size_t fib_cap   = 0;        /* allocated size  */
    static size_t fib_len   = 0;        /* number of stored values */

    /* ensure we have Fib(0)..Fib(n+3) */
    size_t needed_fib = (size_t)n + 4;  /* inclusive of n+3 */
    if (needed_fib > fib_cap) {
        size_t new_cap = fib_cap ? fib_cap : 1;
        while (new_cap < needed_fib) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(fib_cache, new_cap * sizeof(seq_t));
        if (!tmp) { return 0; }  /* OOM; caller may handle */
        fib_cache = tmp;
        fib_cap   = new_cap;
    }
    if (fib_len == 0) {            /* initialise Fib(0)=0, Fib(1)=1 */
        fib_cache[0] = (seq_t)0;
        fib_cache[1] = (seq_t)1;
        fib_len = 2;
    }
    for (size_t i = fib_len; i < needed_fib; ++i) {
        fib_cache[i] = fib_cache[i-1] + fib_cache[i-2];
    }
    fib_len = needed_fib;           /* now fib_cache[0..n+3] exist */

    /* ------------------ a() cache ------------------ */
    static seq_t *a_cache = NULL;    /* a_cache[i] = a(i)   */
    static size_t a_cap   = 0;
    static size_t a_len   = 0;

    /* ensure we can address a[n] */
    if ((size_t)n + 1 > a_cap) {
        size_t new_cap = a_cap ? a_cap : 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        if (!tmp) { return 0; }
        a_cache = tmp;
        a_cap   = new_cap;
    }

    /* initialise first two terms */
    if (a_len == 0) { a_cache[0] = (seq_t)1; a_len = 1; }
    if (a_len == 1 && n >= 1) { a_cache[1] = (seq_t)3; a_len = 2; }

    /* compute missing terms */
    for (idx_t k = (idx_t)a_len; k <= n; ++k) {
        seq_t fn  = fib_cache[(size_t)k];      /* Fib(k)   */
        seq_t fn1 = fib_cache[(size_t)k + 1];  /* Fib(k+1) */
        seq_t fn3 = fib_cache[(size_t)k + 3];  /* Fib(k+3) */

        /* a(k) = a(k-1)*fn3*fn1 / (fn*fn)  */
#if defined(__SIZEOF_INT128__) && !defined(__cplusplus)
        /* 64‑bit build: use 128‑bit intermediate to avoid overflow */
        seq_t t = a_cache[(size_t)k - 1] / fn;   /* exact division */
        unsigned __int128 prod = (unsigned __int128)t * fn3;
        prod = prod * fn1;
        prod = prod / fn;
        a_cache[(size_t)k] = (seq_t)prod;
#else
        /* arbitrary‑precision build: rely on big‑int class */
        seq_t t = a_cache[(size_t)k - 1] / fn;   /* exact division */
        seq_t prod = t * fn3;
        prod = prod * fn1;
        a_cache[(size_t)k] = prod / fn;
#endif
    }
    a_len = (size_t)n + 1;  /* cache now up to n */

    return a_cache[(size_t)n];
}
