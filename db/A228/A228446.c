/*
 * A228446  a(n) = smallest prime p such that 2*n+1 = p + x*(x+1) for some
 *          positive integer x, or -1 if no such prime exists.
 * https://oeis.org/A228446
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 161, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A228446: a(n) = smallest prime p such that 2*n+1 = p + x*(x+1) for some
   positive integer x (x ≥ 1).  For n < 2 the value is undefined and the
   function returns -1. */
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Offset is 2; values for n < 2 are not defined. */
    if (n < 2) return (seq_t)-1;

    /* T = 2*n + 1 */
    seq_t T = (seq_t)n * (seq_t)2 + (seq_t)1;

    seq_t best = (seq_t)-1;           /* no candidate yet            */
    idx_t  x   = 1;                    /* x ≥ 1                       */

    for (;; ++x)
    {
        seq_t xs   = (seq_t)x;
        seq_t prod = xs * (xs + (seq_t)1);   /* x*(x+1)   */

        /* When prod > T-2 no further prime can occur; break. */
        if (prod > T - (seq_t)2) break;

        seq_t p = T - prod;             /* candidate prime          */
        if (is_prime((idx_t)p))
        {
            if (best == (seq_t)-1 || p < best) best = p;
        }
    }

    return best;
}
