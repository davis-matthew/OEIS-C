/*
 * A180664  Golden Triangle sums: a(n) = a(n-1) + A001654(n+1) with a(0)=0.
 * https://oeis.org/A180664
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{i=0}^{n-1} fib(i+2) * fib(i+3), where fib(0)=0,
 *            fib(1)=1
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 45, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)0;

    /* need Fibonacci numbers up to index n+2 */
    const idx_t sz = n + 3;          /* indices 0 .. n+2 inclusive      */
    seq_t *fib = (seq_t*)malloc(sz * sizeof(seq_t));
    if (!fib) return (seq_t)0;       /* allocation failure */

    fib[0] = (seq_t)0;
    fib[1] = (seq_t)1;
    for (idx_t i = 2; i <= n + 2; ++i) {
        fib[i] = fib[i - 1] + fib[i - 2];
    }

    seq_t sum = (seq_t)0;
    for (idx_t i = 0; i < n; ++i) {
        sum += fib[i + 2] * fib[i + 3];
    }

    free(fib);
    return sum;
}
