/*
 * A180363  a(n) = Lucas(prime(n)).
 * https://oeis.org/A180363
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Lucas(prime(n))
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 14, u64 n <= 24, u128 n <= 42,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses fast‑doubling to compute Fibonacci numbers and get_prime() from utils. */

#include "../utils/get_prime.h"
#include <stdint.h>

/* _fib_pair(n, fn, fn1) puts F(n) into *fn and F(n+1) into *fn1.
   F(0)=0, F(1)=1.  Works for idx_t n (≤ 10000) and any seq_t. */
static void
fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn   = 0;
        *fn1  = 1;
        return;
    }
    seq_t a, b;
    fib_pair(n >> 1, &a, &b);          /* (F(k), F(k+1))  where k = ⌊n/2⌋ */

    seq_t two_b = b + b;                /* 2*F(k+1)  */
    seq_t t     = two_b - a;            /* 2*F(k+1)-F(k) */
    seq_t c     = a * t;                /* F(2k)      */
    seq_t d     = a * a + b * b;        /* F(2k+1)    */

    if (n & 1) {                       /* n odd: use (F(2k+1), F(2k+2)) */
        *fn   = d;
        *fn1  = c + d;
    } else {                           /* n even: use (F(2k), F(2k+1)) */
        *fn   = c;
        *fn1  = d;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)            /* undefined for offset 1, but safest fallback */
        return 0;

    uint32_t p = get_prime(n);  /* n‑th prime, 1‑based */

    seq_t fp, fp1;
    fib_pair(p, &fp, &fp1);     /* compute F(p) and F(p+1) */

    seq_t twice_fp1 = fp1 + fp1;/* 2·F(p+1) */
    return twice_fp1 - fp;      /* Lucas(p) = 2·F(p+1) – F(p) */
}
