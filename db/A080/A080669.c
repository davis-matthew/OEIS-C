/*
 * A080669  a(1) = 1; a(2) = 2; a(n) = Fibonacci(a(n-1) + a(n-2)) if n > 2.
 * https://oeis.org/A080669
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=2, a(n)=Fib( a(n-1)+a(n-2) ) for n>2
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    seq_t a1 = (seq_t)1;    /* a(1) */
    seq_t a2 = (seq_t)2;    /* a(2) */
    for (idx_t i = 3; i <= n; ++i) {
        seq_t sum = a1 + a2;          /* a(i-2)+a(i-1) */
        idx_t idx = (idx_t)sum;       /* safe for tested ranges */
        seq_t ai = fib(idx);          /* Fib( sum ) */
        a1 = a2;
        a2 = ai;
    }
    return a2;
}
