/*
 * A099922  a(n) = F(4n) - 2n, where F(n) = Fibonacci numbers A000045.
 * https://oeis.org/A099922
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = F(4*n) - 2*n, where F is Fibonacci with F(0)=0 and F(1)=1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>    /* for bool, not used but allowed */
    
/* Return Fibonacci pair (F(k), F(k+1)) using fast doubling. */
static void fib_pair(idx_t k, seq_t *f0, seq_t *f1)
{
    if (k == 0) {
        *f0 = (seq_t)0;
        *f1 = (seq_t)1;
        return;
    }

    seq_t a = (seq_t)0;   /* F(0)   */
    seq_t b = (seq_t)1;   /* F(1)   */

    /* Compute number of bits in k to iterate from most significant bit. */
    int bits = 0;
    idx_t tmp = k;
    while (tmp) { tmp >>= 1; ++bits; }

    for (int i = bits - 1; i >= 0; --i) {
        /* c = F(2*m)   = a * (2*b - a) */
        /* d = F(2*m+1) = a*a + b*b   */
        seq_t two_b = b + b;            /* 2*b */
        seq_t two_b_minus_a = two_b - a;  /* 2*b - a */
        seq_t c = a * two_b_minus_a;
        seq_t d = a * a + b * b;

        if ((k >> i) & 1) {
            a = d;          /* F(2*m+1) */
            b = c + d;      /* F(2*m+2) = F(2*m+1) + F(2*m) */
        } else {
            a = c;          /* F(2*m)   */
            b = d;          /* F(2*m+1) */
        }
    }

    *f0 = a;
    *f1 = b;
}

seq_t a(idx_t n)
{
    /* Compute F(4*n) */
    idx_t t = n * (idx_t)4;
    seq_t fn, fn1;
    fib_pair(t, &fn, &fn1);

    /* Subtract 2*n */
    seq_t two_n = (seq_t)n * (seq_t)2;
    return fn - two_n;
}
