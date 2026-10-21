/*
 * A075148  Table E(n,k) (listed antidiagonalwise as E(0,0), E(1,0), E(0,1),
 *          E(2,0), E(1,1), E(0,2), ...) where E(n,k) is F(n+k) for all even n
 *          and L(n+k) for all odd n. F(n) and L(n) are the n-th Fibonacci
 *          (A000045) and Lucas (A000032) numbers respectively.
 * https://oeis.org/A075148
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the antidiagonal enumeration of the table ** E(n,k)
 *            where E(n,k)=Fibonacci(n+k) if n is even, ** otherwise
 *            E(n,k)=Lucas(n+k). The sequence is indexed from n=0
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Fast doubling Fibonacci: returns (F(n),F(n+1)) */
static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = (seq_t)0;
        *fn1 = (seq_t)1;
        return;
    }
    seq_t a = 0, b = 1;                     /* (F(0),F(1)) */
    int msb = 63 - __builtin_clzll(n);      /* highest set bit of n */
    for (int i = msb; i >= 0; --i) {
        seq_t c = a * (((seq_t)2 * b) - a); /* F(2k)  = F(k)[2F(k+1)-F(k)] */
        seq_t d = a * a + b * b;           /* F(2k+1)=F(k)^2+F(k+1)^2   */

        if (n & ((idx_t)1 << i)) {         /* bit is 1: advance one step */
            a = d;
            b = c + d;                     /* (F(2k+1),F(2k+2)) */
        } else {                           /* bit is 0: stay at doubled pair */
            a = c;
            b = d;
        }
    }
    *fn  = a;
    *fn1 = b;
}

/* Returns Fibonacci(n) */
static seq_t fib_single(idx_t n)
{
    seq_t f, fp;
    fib_pair(n, &f, &fp);
    return f;
}

/* Returns Lucas(n) using the identity L(n)=2*F(n+1)-F(n) */
static seq_t lucas_single(idx_t n)
{
    seq_t f, fp;
    fib_pair(n, &f, &fp);                  /* fp = F(n+1) */
    return ((seq_t)2 * fp) - f;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* locate diagonal t and offset within that diagonal */
    idx_t t = 0, cum = 0;                    /* cum = total terms before diag t */
    while (n >= cum + (t + 1)) {
        cum += t + 1;
        ++t;
    }
    idx_t offset = n - cum;                  /* 0 ≤ offset ≤ t */
    idx_t row    = t - offset;               /* row index of the term */
    /* row parity determines whether Fibonacci or Lucas is used */
    if ((row & 1) == 0)                       /* even row → Fibonacci */
        return fib_single(t);
    else                                      /* odd row → Lucas */
        return lucas_single(t);
}
