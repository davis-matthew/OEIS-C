/*
 * A203245  Second elementary symmetric function of the first n terms of
 *          (1,2,3,5,8,...).
 * https://oeis.org/A203245
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((F_{n+4} - 2)^2 - (F_{n+2}*F_{n+3} - 1)) / 2 where F_k
 *            are Fibonacci numbers with F_1 = 1, F_2 = 1
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 20, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for idx_t definition provided externally */

/* Fast doubling algorithm for Fibonacci numbers.
   For any non-negative idx_t n it sets *fn = F_n and *fn1 = F_{n+1}. */
static void fib_pair(idx_t n, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = (seq_t)0;
        *fn1 = (seq_t)1;
        return;
    }
    seq_t a, b;
    fib_pair(n >> 1, &a, &b);          /* a = F_k, b = F_{k+1} with k = n/2 */

    seq_t t = ((seq_t)2 * b) - a;      /* 2*F_{k+1} - F_k  (non‑negative) */
    seq_t c = a * t;                   /* F_{2k}            */
    seq_t d = a * a + b * b;           /* F_{2k+1}          */

    if (n & 1) {                       /* odd n */
        *fn  = d;                      /* F_{2k+1}          */
        *fn1 = c + d;                  /* F_{2k+2}          */
    } else {                           /* even n */
        *fn  = c;                      /* F_{2k}            */
        *fn1 = d;                      /* F_{2k+1}          */
    }
}

seq_t a(idx_t n)
{
    /* We need F_{n+4} and F_{n+5}. */
    idx_t k = n + (idx_t)4;
    seq_t F_n4, F_n5;
    fib_pair(k, &F_n4, &F_n5);         /* F_n4 = F_{n+4},  F_n5 = F_{n+5} */

    /* Recover the preceding Fibonacci numbers. */
    seq_t F_n3 = F_n5 - F_n4;          /* F_{n+3} = F_{n+5} - F_{n+4} */
    seq_t F_n2 = F_n4 - F_n3;          /* F_{n+2} = F_{n+4} - F_{n+3} */

    /* Compute the second elementary symmetric sum. */
    seq_t S      = F_n4 - (seq_t)2;           /* S = F_{n+4} - 2      */
    seq_t S_sq   = S * S;                     /* S^2                  */
    seq_t T      = F_n2 * F_n3 - (seq_t)1;    /* T = F_{n+2}*F_{n+3} - 1 */

    seq_t diff   = S_sq - T;                  /* Should be even.      */
    seq_t result = diff / ((seq_t)2);         /* Divide by 2 exactly. */

    return result;
}
