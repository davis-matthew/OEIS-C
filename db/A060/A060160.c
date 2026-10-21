/*
 * A060160  a(n) = 2^n - 1 + Fibonacci(n-1)*2^(n+1).
 * https://oeis.org/A060160
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^n - 1 + Fibonacci(n-1)*2^(n+1)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 19, u64 n <= 38, u128 n <= 76,
 *            big any n
 * Helpers:   fib_pair, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/fib_pair.h"

/* Computes A060160: a(n) = 2^n - 1 + Fib(n-1)*2^(n+1)   (offset 1) */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* behavior below offset is unspecified */

    seq_t fib_nm1, unused;
    fib_pair(n - 1, &fib_nm1, &unused);   /* Fib(n-1) */

    seq_t term = pow2(n);                 /* 2^n               */
    term -= (seq_t)1;                    /* 2^n - 1           */
    term += pow2(n + 1) * fib_nm1;        /* + Fib(n-1)*2^(n+1) */

    return term;
}
