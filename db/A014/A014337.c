/*
 * A014337  Three-fold exponential convolution of Fibonacci numbers with
 *          themselves (divided by 6).
 * https://oeis.org/A014337
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3^n*Fibonacci(n) - 3*Fibonacci(2n)) / 30 Reduced form:
 *            a(n) = (3^{n-1}*Fibonacci(n) - Fibonacci(2n)) / 10 for n ≥ 1;
 *            a(0) = 0
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 16, u64 n <= 30, u128 n <= 57,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* Base case */
    if (n == 0) return (seq_t)0;

    /* Fibonacci values needed */
    seq_t fn  = fib(n);         /* F(n)   */
    seq_t f2n = fib(2 * n);     /* F(2n)  */

    /* For 64‑bit or smaller types we use 128‑bit intermediates to avoid overflow. */
    if (sizeof(seq_t) <= sizeof(uint64_t))
    {
        unsigned __int128 pow = 1;
        for (idx_t i = 1; i < n; ++i)          /* 3^{n‑1} */
            pow *= 3;

        unsigned __int128 fn128  = (unsigned long long)fn;
        unsigned __int128 f2n128 = (unsigned long long)f2n;

        unsigned __int128 term1 = pow * fn128;   /* 3^{n‑1} * F(n) */
        unsigned __int128 diff  = term1 - f2n128;/* numerator            */
        unsigned __int128 res   = diff / 10;      /* final division       */

        return (seq_t)res;
    }
    else
    {
        /* For arbitrary‑precision seq_t we use seq_t arithmetic directly. */
        seq_t pow = 1;
        for (idx_t i = 1; i < n; ++i)          /* 3^{n‑1} */
            pow *= (seq_t)3;

        seq_t term1 = pow * fn;       /* 3^{n‑1} * F(n) */
        seq_t diff  = term1 - f2n;    /* numerator    */
        seq_t res   = diff / (seq_t)10; /* final division */

        return res;
    }
}
