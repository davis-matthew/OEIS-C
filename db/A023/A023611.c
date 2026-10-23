/*
 * A023611  Convolution of Fibonacci numbers and A000201.
 * https://oeis.org/A023611
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0}^{n} Fib(k+1) * h(n-k) where Fib(0)=0, Fib(1)=1,
 *            Fib(k)=Fib(k-1)+Fib(k-2) and h(m)=⌊(m+1)φ⌋ with φ=(1+√5)/2
 * Range:     u8 n <= 7, u16 n <= 18, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  required headers: seq_t and idx_t are provided by the build system */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <math.h>           /* for sqrt() */

/* integer square‑root using double – safe for the small arguments we need */
static inline uint64_t uint64_sqrt(uint64_t x)
{
    return (uint64_t)floor(sqrt((double)x));
}

seq_t a(idx_t n)
{
    /* ------------------------------------------------------------- */
    /* 1.  convert index to native integer for array sizes           */
    /* ------------------------------------------------------------- */
    const size_t N = (size_t)n;           /* N ≥ 0 */

    /* ------------------------------------------------------------- */
    /* 2.  Fibonacci numbers Fib[0..N+1]                            */
    /* ------------------------------------------------------------- */
    seq_t *fib = (seq_t *)malloc((N + 2) * sizeof(seq_t));
    if (!fib) return (seq_t)0;            /* allocation failed – return 0 */

    fib[0] = (seq_t)0;
    fib[1] = (seq_t)1;
    for (size_t i = 2; i <= N + 1; ++i) {
        fib[i] = fib[i-1] + fib[i-2];
    }

    /* ------------------------------------------------------------- */
    /* 3.  Beatty sequence h[0..N]  (floor((m+1)φ))                  */
    /*    Use the identity  floor((u+u√5)/2) with u=m+1              */
    /*    floor(u√5) is floor(√(5u²))                                 */
    /* ------------------------------------------------------------- */
    seq_t *h = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    if (!h) { free(fib); return (seq_t)0; }

    for (size_t m = 0; m <= N; ++m) {
        uint64_t u = (uint64_t)m + 1;                     /* u = m+1 */
        uint64_t u2 = u * u;                              /* u²  ≤ 5·10⁸ for n≤10⁴  */
        uint64_t t  = 5 * u2;                             /* 5u² */
        uint64_t s  = uint64_sqrt(t);                     /* floor(u√5) */
        h[m] = ((seq_t)u + (seq_t)s) / (seq_t)2;          /* floor((u+s)/2) */
    }

    /* ------------------------------------------------------------- */
    /* 4.  Convolution: a(n) = Σ_{k=0}^{n} Fib(k+1) * h(n-k)        */
    /* ------------------------------------------------------------- */
    seq_t result = (seq_t)0;
    for (size_t k = 0; k <= N; ++k) {
        result += fib[k + 1] * h[N - k];
    }

    /* ------------------------------------------------------------- */
    /* 5.  Clean up                                               */
    /* ------------------------------------------------------------- */
    free(fib);
    free(h);

    return result;
}
