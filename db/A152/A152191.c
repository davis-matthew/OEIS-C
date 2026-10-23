/*
 * A152191  a(n)=Product_{k=1..floor((n-1)/2)} (1 + 4*cos(k*Pi/n)^2)*(1 -
 *          4*sin(k*Pi/n)^2).
 * https://oeis.org/A152191
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 12, i16 n <= 24, i32 n <= 40, i64 n <= 40, i128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A152191
   Formula: a(0)=1; for n>0, if n mod 6 == 0 then a(n)=0.
   Otherwise a(n)=Fibonacci(n) multiplied by a sign factor:
         n mod 6 == 1,2,5  ->  +1
         n mod 6 == 3      ->  -2
         n mod 6 == 4      ->  -1
   where F(n) is the n-th Fibonacci number (F(0)=0, F(1)=1). */

#include <stdint.h>

/* Computes (F(n), F(n+1)) using fast doubling. */
static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1;
        return;
    }
    seq_t a, b;
    fib_pair(n >> 1, &a, &b);
    seq_t two_b = b + b;
    seq_t c = a * (two_b - a);          /* F(2k)   */
    seq_t d = a * a + b * b;            /* F(2k+1) */
    if ((n & 1) == 0) {
        *fn  = c;
        *fn1 = d;
    } else {
        *fn  = d;
        *fn1 = c + d;
    }
}

/* OEIS A152191 */
seq_t a(idx_t n)
{
    if (n == 0) {
        return 1;          /* a(0) = 1 */
    }
    if ((n % 6u) == 0) {
        return 0;          /* multiples of 6 (except 0) are 0 */
    }

    seq_t f, f1;
    fib_pair(n, &f, &f1);  /* f = F(n) */

    uint64_t mod = n % 6u;
    switch (mod) {
        case 1:
        case 2:
        case 5:
            return f;                 /* +1 * F(n) */
        case 3:
            return -(f + f);          /* -2 * F(n) */
        case 4:
            return -f;                /* -1 * F(n) */
        default:
            return 0; /* unreachable */
    }
}
