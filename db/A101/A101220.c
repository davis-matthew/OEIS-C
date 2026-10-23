/*
 * A101220  a(n) = Sum_{k=0..n} Fibonacci(n-k)*n^k.
 * https://oeis.org/A101220
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} Fibonacci(n-k)*n^k
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Build Fibonacci array up to F_n */
    size_t sz = (size_t)n + 1;
    seq_t *fib = (seq_t*)malloc(sz * sizeof(seq_t));
    if (!fib) return 0;             /* very unlikely for n <= 10000 */

    fib[0] = 0;
    if (n >= 1) fib[1] = 1;
    for (idx_t i = 2; i <= n; ++i) {
        fib[i] = fib[i-1] + fib[i-2];
    }

    /* Accumulate the sum Σ_{k=0..n} F_{n-k} * n^k */
    seq_t sum = 0;
    seq_t pow = 1;                 /* n^0 */
    for (idx_t k = 0; k <= n; ++k) {
        sum += fib[n-k] * pow;
        pow *= (seq_t)n;            /* next power n^(k+1) */
    }

    free(fib);
    return sum;
}
