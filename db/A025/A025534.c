/*
 * A025534  a(n) = (1/C(n,0) + 1/C(n,1) + ... + 1/C(n,k))*L, where k = [ n/2
 *          ], L = LCM{C(n,0), C(n,1),..., C(n,n)}.
 * https://oeis.org/A025534
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/C(n,0)+1/C(n,1)+...+1/C(n,⌊n/2⌋)) *
 *            LCM{C(n,0),C(n,1),...,C(n,n)}
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 27, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function computes the values exactly using the provided
   arbitrary-precision arithmetic routines and a simple O(n)
   algorithm. */

#include <stddef.h>                 /* for size_t */
#include "../utils/gcd.h"           /* static seq_t gcd(seq_t, seq_t) */

seq_t a(idx_t n)
{
    /* n = 0 is trivial: C(0,0)=1, L=1, sum=1 */
    if (n == 0)
        return (seq_t)1;

    /* ---- compute LCM of all binomial coefficients ---- */
    seq_t L = (seq_t)1;           /* LCM so far, starts with 1 (C(n,0)) */
    seq_t binom = (seq_t)1;       /* current binomial C(n,i) */

    for (idx_t i = 1; i <= n; ++i) {
        /* binom = binom * (n - i + 1) / i  */
        binom = (binom * (seq_t)(n - i + 1)) / (seq_t)i;

        seq_t g = gcd(L, binom);
        L = (L / g) * binom;     /* lcm(L, binom) */
    }

    /* ---- compute the required sum using the final L ---- */
    idx_t k = n / 2;              /* floor(n/2) */
    seq_t sum = (seq_t)0;

    binom = (seq_t)1;             /* reset to C(n,0) for the sum */

    /* i = 0 term */
    sum += L / binom;             /* L / 1 */

    for (idx_t i = 1; i <= k; ++i) {
        binom = (binom * (seq_t)(n - i + 1)) / (seq_t)i;
        sum += L / binom;
    }

    return sum;
}
