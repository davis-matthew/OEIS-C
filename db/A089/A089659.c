/*
 * A089659  a(n) = S1(n,2), where S1(n, t) = Sum_{k=0..n} (k^t * Sum_{j=0..k}
 *          binomial(n,j)).
 * https://oeis.org/A089659
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-3) * n * (7*n^2 + 12*n + 5) / 3 (n ≥ 3) * Initial
 *            values: a(0) = 0, a(1) = 2, a(2) = 19, a(3) = 104 * * Uses the
 *            explicit formula for all n ≥ 3; for n < 3 the constants are *
 *            returned directly so that no negative exponent of 2 is
 *            evaluated. * * Requires the utility function pow2(idx_t...
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 20, u64 n <= 48, u128 n <= 109,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* handle the first few terms that are defined separately */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)19;
    if (n == 3) return (seq_t)104;

    /* n >= 4 : use the closed‑form expression */
    seq_t tn   = (seq_t)n;                    /* n                */
    seq_t tn2  = tn * tn;                     /* n^2              */
    seq_t sum  = (seq_t)7 * tn2 + (seq_t)12 * tn + (seq_t)5; /* 7*n^2 + 12*n + 5 */
    seq_t prod = tn * sum;                    /* n*(7*n^2 + 12*n + 5) */
    prod /= (seq_t)3;                         /* exact division by 3 */

    seq_t pow   = pow2(n - 3);                /* 2^(n-3)          */
    return pow * prod;                        /* final result */
}
