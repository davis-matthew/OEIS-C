/*
 * A054849  a(n) = 2^(n-5)*binomial(n,5). Number of 5D hypercubes in an
 *          n-dimensional hypercube.
 * https://oeis.org/A054849
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^(n-5) * binomial(n,5)
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 22, u64 n <= 48, u128 n <= 106,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (offset = 5, a(5)=1) */

#include <stdint.h>
#include "../utils/pow2.h"   /* provides seq_t pow2(idx_t exp) */

seq_t a(idx_t n)
{
    /* For n < 5 the sequence is undefined; return 0 safely. */
    if (n < 5)
        return 0;

    /* Base value */
    if (n == 5)
        return 1;

    /* Compute binomial(n,5) = n*(n-1)*(n-2)*(n-3)*(n-4) / 120.
       Use an intermediate 128‑bit unsigned integer to avoid overflow
       on the intermediate product.  The result fits in seq_t for
       all n tested on the 64‑bit build. */
    unsigned __int128 prod = (unsigned __int128)n;
    prod *= (n - 1);
    prod *= (n - 2);
    prod *= (n - 3);
    prod *= (n - 4);
    prod /= 120;                     /* 5! = 120 */

    seq_t bin = (seq_t)prod;

    /* 2^(n-5) computed via the utility function.  The returned type
       matches seq_t and handles arbitrary precision automatically. */
    seq_t p2 = pow2(n - 5);

    return p2 * bin;
}
