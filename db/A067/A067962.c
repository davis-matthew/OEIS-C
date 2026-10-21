/*
 * A067962  a(n) = F(n+2)*(Product_{i=1..n+1} F(i))^2 where F(i)=A000045(i) is
 *          the i-th Fibonacci number.
 * https://oeis.org/A067962
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = F(n+2)*(Product_{i=1..n+1} F(i))^2 Recurrence: a(0)=1;
 *            a(n)=a(n-1)*F(n+1)*F(n+2) for n>0
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    /* allocate array for Fibonacci numbers up to F(n+2) */
    size_t sz = (size_t)n + 3;           /* indices 0 .. n+2 inclusive */
    seq_t *fib = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!fib)
        return 0;                       /* allocation failed */

    fib[0] = 0;
    fib[1] = 1;
    for (idx_t i = 2; i <= n + 2; ++i)
        fib[i] = fib[i - 1] + fib[i - 2];

    seq_t result = 1;
    for (idx_t i = 1; i <= n; ++i) {
        result *= fib[i + 1];
        result *= fib[i + 2];
    }

    free(fib);
    return result;
}
