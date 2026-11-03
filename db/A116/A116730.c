/*
 * A116730  Number of permutations of length n which avoid the patterns 321,
 *          1342, 1423.
 * https://oeis.org/A116730
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*F_{n+3} - 3*n - 2
 * Range:     u8 n <= 8, u16 n <= 20, u32 n <= 43, u64 n <= 89, u128 n <= 181,
 *            big any n
 * Validated: standard, pass, 53 terms through n = 961 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static seq_t fib(idx_t n) {
    if (n <= 2) return (seq_t)1;
    seq_t f1 = 1, f2 = 1, f3;
    for (idx_t i = 3; i <= n; ++i) {
        f3 = f1 + f2;
        f1 = f2;
        f2 = f3;
    }
    return f2;
}

seq_t a(idx_t n)
{
    /*  a(n) = 2 * Fibonacci(n+3) - 3*n - 2,  with Fibonacci(0)=0, Fibonacci(1)=1 */
    seq_t f = fib(n + 3);                /* F_{n+3} */
    seq_t result = f * 2;                /* 2*F_{n+3} */
    result -= (seq_t)n * 3;              /* subtract 3*n */
    result -= 2;                         /* subtract 2 */
    return result;
}
