/*
 * A099164  a(n) = (L(n+2) + 2*3^n)/5.
 * https://oeis.org/A099164
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (L(n+2) + 2*3^n)/5 L(k) is the k-th Lucas number: L(0)=2,
 *            L(1)=1, L(k)=L(k-1)+L(k-2)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Fast doubling method for Fibonacci numbers.
   After call fn = F(n), fn1 = F(n+1) where F(0)=0, F(1)=1. */
static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = (seq_t)0;
        *fn1 = (seq_t)1;
        return;
    }
    seq_t a,b;
    fib_pair(n >> 1, &a, &b);          /* a = F(k), b = F(k+1)  */
    seq_t two_b_minus_a = (seq_t)2 * b - a;  /* 2*F(k+1)-F(k)         */
    seq_t c = a * two_b_minus_a;      /* F(2k) */
    seq_t d = a * a + b * b;           /* F(2k+1) */
    if ((n & 1) == 0) {
        *fn  = c;
        *fn1 = d;
    } else {
        *fn  = d;
        *fn1 = c + d;
    }
}

/* Power of 3: 3^e */
static seq_t pow3(idx_t e)
{
    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)3;
    while (e) {
        if (e & 1) result *= base;
        base *= base;
        e >>= 1;
    }
    return result;
}

/* Lucas number via Fibonacci: L(n) = F(n-1) + F(n+1) for n>=1,
   L(0)=2. */
static seq_t lucas(idx_t n)
{
    if (n == 0) return (seq_t)2;
    /* compute F(n-1) and F(n+1) */
    seq_t fn_1, fn, fn1;
    fib_pair(n-1, &fn_1, &fn);          /* fn_1 = F(n-1), fn = F(n)  */
    /* we already have fn, fn1 = F(n+1)          */
    seq_t fn_2, fn_3;
    fib_pair(n+1, &fn, &fn1);           /* fn = F(n+1), fn1 = F(n+2) */
    /* L(n) = F(n-1) + F(n+1) */
    return fn_1 + fn;
}

seq_t a(idx_t n)
{
    /* Handle small n explicitly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)5;

    /* Compute L(n+2) = F(n+1) + F(n+3) (for n >= 0) */
    seq_t fn_n1, fn_n2, fn_n3, fn_n4;
    fib_pair(n+1, &fn_n1, &fn_n2);     /* fn_n1 = F(n+1), fn_n2 = F(n+2) */
    fib_pair(n+3, &fn_n3, &fn_n4);     /* fn_n3 = F(n+3), fn_n4 = F(n+4) */
    seq_t L = fn_n1 + fn_n3;           /* L(n+2) */

    /* Compute 3^n */
    seq_t p3 = pow3(n);

    /* Compute (L + 2*3^n)/5 */
    seq_t two_p3 = (seq_t)2 * p3;
    seq_t total  = L + two_p3;
    return total / (seq_t)5;
}
