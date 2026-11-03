/*
 * A033814  Convolution of positive integers n with Lucas numbers
 *          L(k)(A000032) for k >= 4.
 * https://oeis.org/A033814
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = L(n+7) - 11*n - 29, where L is the Lucas sequence
 *            (L(0)=2, L(1)=1). The Lucas number is obtained from Fibonacci
 *            numbers: L(k) = F(k-1) + F(k+1). Hence a(n) = F(n+6) + F(n+8) -
 *            11*n - 29
 * Range:     u8 n <= 5, u16 n <= 16, u32 n <= 39, u64 n <= 85, u128 n <= 177,
 *            big any n
 * Validated: standard, pass, 50 terms through n = 869 (2026-11-01)
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
    /* Compute Fibonacci numbers needed for Lucas (n+7) */
    seq_t f_n6 = fib(n + 6);   /* F(n+6)   */
    seq_t f_n8 = fib(n + 8);   /* F(n+8)   */
    seq_t L_n7 = f_n6 + f_n8;  /* L(n+7) */

    seq_t sn = (seq_t)n;
    return L_n7 - 11 * sn - 29;
}
