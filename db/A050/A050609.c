/*
 * A050609  Table T(n,k) = Sum_{i=0..2n} (C(2n,i) mod 2)*F(i+k) = Sum_{i=0..n}
 *          (C(n,i) mod 2)*F(2i+k).
 * https://oeis.org/A050609
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   for every n >= 0 the value a(n) is obtained by antidiagonal
 *            traversal of the table T(i,j) = Σ_{t=0..i} (C(i,t) mod 2) * F(2t
 *            + j) where F(k) is the Fibonacci number with F(0)=0, F(1)=1. The
 *            traversal is: T(0,0), T(1,0), T(0,1), T(2,0), T(1,1), T(0,2), …
 * Range:     u8 n <= 27, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* fast doubling – returns (F(n),F(n+1)) in *f and *g */
static void fib_pair_rec(idx_t n, seq_t *f, seq_t *g)
{
    if (n == 0)
    {
        *f = (seq_t)0;
        *g = (seq_t)1;
        return;
    }
    seq_t a, b, c, d, two_b;
    fib_pair_rec(n >> 1, &a, &b);          /* a = F(k), b = F(k+1) */
    two_b = b + b;                         /* 2*b */
    c = a * (two_b - a);                  /* F(2k)  */
    d = a * a + b * b;                    /* F(2k+1) */
    if (n & 1)
    {
        *f = d;                            /* F(2k+1) */
        *g = c + d;                        /* F(2k+2) */
    }
    else
    {
        *f = c;                            /* F(2k) */
        *g = d;                            /* F(2k+1) */
    }
}

/* compute F(m) via fast doubling */
static inline seq_t fib_fast(idx_t m)
{
    seq_t f, g;
    fib_pair_rec(m, &f, &g);
    return f;
}

/* a(n) – sequence A050609 */
seq_t a(idx_t n)
{
    /* find diagonal s with s*(s+1)/2 <= n < (s+1)*(s+2)/2 */
    idx_t s = 0;
    while ( ((unsigned __int128)(s + 1) * (s + 2)) / 2 <= n )
        ++s;

    idx_t offset = n - ((s * (s + 1)) / 2);
    idx_t i = s - offset;   /* table row index  */
    idx_t j = offset;       /* table column index */

    seq_t sum = (seq_t)0;
    for (idx_t t = 0; t <= i; ++t)
    {
        if ( (t & (~i)) == 0 )         /* C(i,t) is odd iff t is a subset of i */
        {
            idx_t idx = 2 * t + j;
            sum += fib_fast(idx);
        }
    }
    return sum;
}
