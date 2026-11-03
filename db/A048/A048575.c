/*
 * A048575  Pisot sequences L(2,5), E(2,5).
 * https://oeis.org/A048575
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = F_{2n+3} (Fibonacci number)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 22, u64 n <= 45, u128 n <= 91,
 *            big any n
 * Validated: standard, pass, 45 terms through n = 991 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  or  a(0)=2, a(1)=5, a(n)=3a(n-1)-a(n-2)  */

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
    /* Compute Fibonacci(2*n+3) */
    idx_t m = n << 1;   /* 2*n  */
    m += 3;             /* 2*n+3  */
    return fib(m);
}
