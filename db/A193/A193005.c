/*
 * A193005  Coefficient of x in the reduction by x^2->x+1 of the polynomial
 *          p(n,x) defined at Comments.
 * https://oeis.org/A193005
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*F(n+4) + 4*F(n+5) - 50 - 24*n - 6*n^2 - n^3 where F(k)
 *            is the k-th Fibonacci number (F(0)=0, F(1)=1). The result is
 *            returned as type seq_t
 * Range:     u8 n <= 5, u16 n <= 15, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Recursive fast‑doubling Fibonacci: returns (F(n), F(n+1)) */
static void fib_pair(idx_t n, seq_t *f, seq_t *f_next)
{
    if (n == 0) {
        *f = (seq_t)0;
        *f_next = (seq_t)1;
        return;
    }

    seq_t a, b;
    fib_pair(n >> 1, &a, &b);           /* k = floor(n/2), a=F(k), b=F(k+1) */

    /* c = F(2k) = a * (2*b - a) */
    seq_t two_b = b + b;                /* 2*b */
    seq_t two_b_minus_a = two_b - a;    /* 2*b - a */
    seq_t c = a * two_b_minus_a;        /* a * (2*b - a) */

    /* d = F(2k+1) = a^2 + b^2 */
    seq_t d = (a * a) + (b * b);

    if (n & 1) { /* n is odd: F(n)=d, F(n+1)=c+d */
        *f = d;
        *f_next = c + d;
    } else {      /* n is even: F(n)=c, F(n+1)=d */
        *f = c;
        *f_next = d;
    }
}

seq_t a(idx_t n)
{
    /* Compute Fibonacci numbers needed for the closed form */
    seq_t f_n4, f_n4_next;
    fib_pair(n + 4, &f_n4, &f_n4_next);    /* F(n+4) -> f_n4 */

    seq_t f_n5, f_n5_next;
    fib_pair(n + 5, &f_n5, &f_n5_next);    /* F(n+5) -> f_n5 */

    /* result = 10*F(n+4) + 4*F(n+5) */
    seq_t result = f_n4 * (seq_t)10;
    result += f_n5 * (seq_t)4;

    /* Subtract polynomial terms: -50 - 24*n - 6*n^2 - n^3 */
    seq_t n_seq = (seq_t)n;
    seq_t n2 = n_seq * n_seq;          /* n^2 */
    seq_t n3 = n2 * n_seq;              /* n^3 */
    seq_t n24 = n_seq * (seq_t)24;       /* 24*n */
    seq_t n6 = n2 * (seq_t)6;            /* 6*n^2 */

    result -= (seq_t)50;
    result -= n24;
    result -= n6;
    result -= n3;

    return result;
}
