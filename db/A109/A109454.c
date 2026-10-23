/*
 * A109454  Sum of non-Fibonacci numbers between successive Fibonacci numbers:
 *          a(n) = Sum_{k=F(n)+1..F(n+1)-1} k.
 * https://oeis.org/A109454
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(n+2)*(Fibonacci(n-1)-1)/2 for n>1,
 *            a(0)=a(1)=a(2)=a(3)=0
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 24, u64 n <= 47, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static inline void fib_doubling(idx_t n, seq_t *f, seq_t *f_next)
{
    if (n == 0) {
        *f = (seq_t)0;
        *f_next = (seq_t)1;
        return;
    }
    seq_t a, b;
    fib_doubling(n >> 1, &a, &b);
    seq_t two_b = b + b;                /* 2*F(k+1) */
    seq_t c = a * (two_b - a);          /* F(2k)   */
    seq_t d = a * a + b * b;            /* F(2k+1) */
    if ((n & 1) == 0) {
        *f = c;
        *f_next = d;
    } else {
        *f = d;
        *f_next = c + d;
    }
}

seq_t a(idx_t n)
{
    /* Handle the small initial values explicitly */
    if (n <= 3) return (seq_t)0;

    seq_t fn_plus2;    /* Fibonacci(n+2) */
    seq_t tmp;
    fib_doubling(n + 2, &fn_plus2, &tmp);

    seq_t fn_minus1;   /* Fibonacci(n-1) */
    fib_doubling(n - 1, &fn_minus1, &tmp);

    seq_t temp = fn_minus1 - (seq_t)1;
    seq_t prod = fn_plus2 * temp;
    return prod / (seq_t)2;
}
