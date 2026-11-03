/*
 * A050611  Sum_{i=0..n} (C(n,i) mod 2)*Fibonacci(2i+1) =
 *          FL(n+1)Product(L(2^i)^bit(n,i),i=0..).
 * https://oeis.org/A050611
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i⊆n} Fib(2*i+1) where C(n,i) mod 2 = 1 iff i is a
 *            submask of n
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Validated: partial, pass, 28 terms through n = 27 (2026-11-01)
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
    seq_t sum = 0;
    idx_t sub = n;
    while (1) {
        /* Compute Fibonacci(2*sub + 1) and add to the sum. */
        sum += fib((sub << 1) + 1);
        if (sub == 0) break;
        sub = (sub - 1) & n;   /* iterate over all submasks of n */
    }
    return sum;
}
