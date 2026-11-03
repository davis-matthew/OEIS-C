/*
 * A047790  a(n) = Fibonacci(2*n)-2^n+1.
 * https://oeis.org/A047790
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(2*n) - 2^n + 1
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 23, u64 n <= 46, u128 n <= 93,
 *            big any n
 * Validated: standard, pass, 47 terms through n = 889 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

/* Fast doubling algorithm to compute Fibonacci numbers.
   Computes F_n and F_{n+1} for n >= 0, with F_0 = 0, F_1 = 1. */
static inline void fib_pair_custom(idx_t n, seq_t *f_n, seq_t *f_n1)
{
    if (n == 0) {
        *f_n  = 0;
        *f_n1 = 1;
        return;
    }
    seq_t a, b;                            /* a = F_k, b = F_{k+1}   */
    fib_pair_custom(n >> 1, &a, &b);        /* recurse on k = n/2      */
    seq_t two_b = b << 1;                   /* 2*F_{k+1}              */
    seq_t c = a * (two_b - a);              /* F_{2k}                  */
    seq_t d = a * a + b * b;                /* F_{2k+1}                */
    if ((n & 1) == 0) {                     /* n is even              */
        *f_n  = c;
        *f_n1 = d;
    } else {                                /* n is odd               */
        *f_n  = d;
        *f_n1 = c + d;
    }
}

seq_t a(idx_t n)
{
    /* compute F_{2n} */
    seq_t f, fp1;
    fib_pair_custom(n << 1, &f, &fp1);

    /* compute 2^n */
    seq_t pow2 = ipow((seq_t)2, (seq_t)n);

    /* a(n) = F_{2n} - 2^n + 1 */
    return f - pow2 + 1;
}
