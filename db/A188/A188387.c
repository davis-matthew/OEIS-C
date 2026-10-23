/*
 * A188387  Central coefficient in (1 + (2^n+1)*x + 2^n*x^2)^n for n>=0.
 * https://oeis.org/A188387
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} C(n,k)^2 * 2^(n*k)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/pow2.h"

/* Compute the OEIS sequence A188387: central coefficient in (1 + (2^n + 1)*x + 2^n*x^2)^n */
seq_t a(idx_t n)
{
    seq_t sum    = 0;                     /* accumulator for the sum   */
    seq_t binom  = 1;                     /* current binomial C(n,k)   */
    seq_t pow2n  = pow2(n);               /* 2^n, used to build 2^(n*k)  */
    seq_t pownk  = 1;                     /* 2^(n*k) for the current k   */

    for (idx_t k = 0; k <= n; ++k) {
        if (k > 0)                         /* update 2^(n*k) */
            pownk *= pow2n;

        seq_t term = binom * binom;       /* C(n,k)^2       */
        term *= pownk;                    /* C(n,k)^2 * 2^(n*k) */

        sum += term;                      /* accumulate the term   */

        if (k == n)
            break;                         /* last iteration finished */

        /* compute next binomial coefficient C(n,k+1) */
        /* multiplication followed by division gives an exact integer */
        binom = binom * (seq_t)(n - k) / (seq_t)(k + 1);
    }

    return sum;
}
