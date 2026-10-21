/*
 * A081003  a(n) = Fibonacci(4n+1) + 1, or Fibonacci(2n+1)*Lucas(2n).
 * https://oeis.org/A081003
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(4n+1) + 1 Computation is done via fast
 *            doubling, which works for all seq_t types (arbitrary‑precision
 *            or native integer)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"   /* only for type definitions, no function prototypes */

static void fib_fast(idx_t n, seq_t *fn, seq_t *fn1)
/* Compute Fibonacci numbers F(n) and F(n+1) via fast doubling.
   All arithmetic is performed with seq_t operations. */
{
    if (n == 0) {
        *fn = (seq_t)0;
        *fn1 = (seq_t)1;
        return;
    }

    seq_t f_k, f_k1;
    fib_fast(n >> 1, &f_k, &f_k1);      /* recursive step on floor(n/2) */

    /* c = F(2k)   = F(k) * [2*F(k+1) – F(k)] */
    seq_t two_fk1 = f_k1 + f_k1;
    seq_t two_fk1_minus_fk = two_fk1 - f_k;
    seq_t c = f_k * two_fk1_minus_fk;

    /* d = F(2k+1) = F(k)^2 + F(k+1)^2 */
    seq_t f_k_sq = f_k * f_k;
    seq_t f_k1_sq = f_k1 * f_k1;
    seq_t d = f_k_sq + f_k1_sq;

    if (n & 1) {        /* n is odd: F(2k+1), F(2k+2) */
        *fn = d;
        *fn1 = c + d;
    } else {            /* n is even: F(2k), F(2k+1) */
        *fn = c;
        *fn1 = d;
    }
}

seq_t a(idx_t n)
{
    idx_t k = (idx_t)(4 * n + 1);      /* compute the argument 4n+1 */
    seq_t fn, fn1;
    fib_fast(k, &fn, &fn1);            /* fn = F(k), fn1 = F(k+1) */
    return fn + (seq_t)1;              /* a(n) = F(4n+1) + 1 */
}
