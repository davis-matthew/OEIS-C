/*
 * A121364  Convolution of A066983 with the double Fibonacci sequence A103609.
 * https://oeis.org/A121364
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = F(n) – F(⌊(n‑1)/2⌋ + 1) (F = Fibonacci)
 * Range:     u8 n <= 13, u16 n <= 24, u32 n <= 47, u64 n <= 93, u128 n <= 186,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *fib_cache = NULL;   /* Fibonacci numbers */
static idx_t fib_last = 0;        /* highest Fibonacci index cached */

/* Ensure that fib_cache[0..n] are filled. */
static void ensure_fib(idx_t n)
{
    if (fib_cache == NULL) {
        size_t sz = (size_t)(n + 1);
        if (sz < 2) sz = 2;
        fib_cache = (seq_t *)malloc(sz * sizeof(seq_t));
        if (fib_cache == NULL) { exit(1); }
        fib_cache[0] = 0;          /* F(0) */
        fib_cache[1] = 1;          /* F(1) */
        fib_last = 1;
    } else if (fib_last < n) {
        size_t sz = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(fib_cache, sz * sizeof(seq_t));
        if (tmp == NULL) { exit(1); }
        fib_cache = tmp;
        for (idx_t i = fib_last + 1; i <= n; ++i) {
            fib_cache[i] = fib_cache[i - 1] + fib_cache[i - 2];
        }
        fib_last = n;
    }
}

seq_t a(idx_t n)
{
    /* a(n) = F(n) – F( floor((n‑1)/2) + 1 ) */
    if (n == 0) { return 0; }  /* not a valid index, but safe */

    /* Fibonacci needed up to n and up to floor((n‑1)/2)+1 (≤ n) */
    ensure_fib(n);

    seq_t fn = fib_cache[n];
    idx_t k = (n - 1) / 2 + 1;
    seq_t fk = fib_cache[k];
    return fn - fk;
}
