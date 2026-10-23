/*
 * A119737  a(n) = Sum_{k=1..n} Fibonacci(floor(n/k)).
 * https://oeis.org/A119737
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=1..n} Fibonacci(floor(n/k))
 * Range:     u8 n <= 13, u16 n <= 24, u32 n <= 47, u64 n <= 93, u128 n <= 186,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Fibonacci is defined by F(0)=0, F(1)=1, F(m)=F(m-1)+F(m-2). */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* handle trivial case */
    if (n == 0) {
        return 0;
    }

    /* Allocate array for Fibonacci numbers up to F(n) */
    seq_t *fib = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!fib) {
        /* In a real project we might handle out‑of‑memory more gracefully */
        return 0;
    }
    fib[0] = 0;
    if (n >= 1) {
        fib[1] = 1;
    }

    /* compute F(i) for i = 2..n */
    for (idx_t i = 2; i <= n; ++i) {
        fib[i] = fib[i - 1] + fib[i - 2];
    }

    /* sum Fibonacci(floor(n/k)) for k=1..n */
    seq_t sum = 0;
    for (idx_t v = 1; v <= n; ++v) {
        idx_t cnt = n / v - n / (v + 1);
        if (cnt) {
            /* multiply fib[v] by cnt */
            seq_t term = fib[v] * (seq_t)cnt;
            sum = sum + term;
        }
    }

    free(fib);
    return sum;
}
