/*
 * A122931  Row sums of triangular array A122930.
 * https://oeis.org/A122931
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (F(n) * (F(n+3)-1)) / 2, where F(k) is the k-th Fibonacci
 *            number
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 46, u128 n <= 93,
 *            big any n
 * Validated: standard, pass, 44 terms through n = 850 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
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

seq_t a(idx_t n) {
    seq_t Fn   = fib(n);       /* F(n)   */
    seq_t Fn3  = fib(n + 3);   /* F(n+3) */
    seq_t Fn3m1 = Fn3 - 1;     /* F(n+3)-1 */

    /* One of Fn or (Fn3-1) must be even; divide that one by 2 first
       to keep intermediate values small. */
    if ((Fn3m1 & 1) == 0) {
        return Fn * (Fn3m1 >> 1); /* (F(n) * ((F(n+3)-1)/2)) */
    } else {
        return (Fn >> 1) * Fn3m1; /* ((F(n)/2) * (F(n+3)-1)) */
    }
}
