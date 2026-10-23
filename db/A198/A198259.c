/*
 * A198259  a(n) = Sum_{k=1..n} F(n mod k) where F = A000045, the Fibonacci
 *          numbers.
 * https://oeis.org/A198259
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} F(n mod k), where F is the Fibonacci
 *            sequence (F(0)=0, F(1)=1)
 * Range:     u8 n <= 22, u16 n <= 46, u32 n <= 92, u64 n <= 184,
 *            u128 n <= 370, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute a(n) as specified in the OEIS entry A198259. */
seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1, but safe for 0 */
        return 0;

    /* Allocate storage for Fibonacci numbers F[0..n-1]. */
    seq_t *fib = (seq_t *)malloc(sizeof(seq_t) * n);
    if (!fib)
        return 0;        /* allocation failure → return 0 as a safe fallback */

    /* Build Fibonacci table up to index n-1. */
    fib[0] = 0;
    if (n > 1)
        fib[1] = 1;
    for (idx_t i = 2; i < n; ++i)
        fib[i] = fib[i - 1] + fib[i - 2];

    /* Sum F(n mod k) for k = 1..n. */
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        idx_t r = n % k;      /* 0 <= r < k <= n-1, fits in fib array */
        sum += fib[r];
    }

    free(fib);
    return sum;
}
