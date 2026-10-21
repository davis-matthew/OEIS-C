/*
 * A056589  Third column sequence of unsigned triangle A056588.
 * https://oeis.org/A056589
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = -2^(n+3) + F(n+4)*(F(n+3)+1) where F(k) is the k‑th
 *            Fibonacci number with F(0)=0, F(1)=1
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 21, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   fib_pair, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"      /* pow2(exp) returns 2^exp as seq_t   */
#include "../utils/fib_pair.h"  /* fib_pair(k, &Fk, &Fk1) -> F(k),F(k+1) */

seq_t a(idx_t n)
{
    seq_t fn3, fn4;               /* fn3 = F(n+3), fn4 = F(n+4)           */
    fib_pair(n + 3, &fn3, &fn4);  /* compute both Fibonacci values       */

    seq_t two_pow = pow2(n + 3);   /* 2^(n+3)                              */
    seq_t temp    = fn3 + 1;       /* F(n+3)+1                             */
    seq_t prod    = fn4 * temp;    /* F(n+4)*(F(n+3)+1)                    */
    seq_t result  = prod - two_pow;/* final result                          */

    return result;
}
