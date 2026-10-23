/*
 * A123742  Certain Vandermonde determinants with Fibonacci numbers.
 * https://oeis.org/A123742
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (-1)^(n-1) * (∏_{i=1}^{n-1} (F(n+1)-F(i+1))) * a(n-1)
 *            with a(1)=1. F(k) is the k‑th Fibonacci number (F(1)=1,F(2)=1)
 * Range:     i8 n <= 4, i16 n <= 5, i32 n <= 6, i64 n <= 7, i128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate array for Fibonacci numbers up to F(n+1).  Index mind: F(k)=fib[k] */
    seq_t *fib = (seq_t *)malloc(sizeof(seq_t) * (size_t)(n + 2));
    fib[0] = (seq_t)0;
    fib[1] = (seq_t)1;
    for (idx_t i = 2; i <= n + 1; ++i)
        fib[i] = fib[i - 1] + fib[i - 2];

    if (n == 0) {   /* n should be >=1 by offset, but handle safely */
        free(fib);
        return (seq_t)0;
    }

    seq_t prev = (seq_t)1; /* a(1) */

    for (idx_t k = 2; k <= n; ++k) {
        seq_t prod = (seq_t)1;
        seq_t newFib = fib[k + 1];                /* F(k+1) */

        for (idx_t i = 1; i < k; ++i) {          /* i = 1 .. k-1 */
            prod = prod * (newFib - fib[i + 1]); /* difference positive */
        }

        seq_t cur = prod * prev;
        if (((k - 1) & 1) != 0)                    /* apply (-1)^(k-1) */
            cur = -cur;

        prev = cur;
    }

    free(fib);
    return prev;
}
