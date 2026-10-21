/*
 * A224465  The Wiener index of the nanostar dendrimer defined pictorially as
 *          NS[n] in the Z. S. Irani reference.
 * https://oeis.org/A224465
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9180 + 2^n*(2800*n+136)+4^n*(320*n-976)
 * Range:     u8 none, u16 n <= 2, u32 n <= 10, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"          /* provides seq_t pow2(idx_t exp) */

seq_t a(idx_t n)
{
    /* special‑case n = 0 to avoid unsigned underflow */
    if (n == 0u)
        return (seq_t)8340;

    idx_t two_n = n + n;           /* 2*n  – safe for small n */
    seq_t pow2n   = pow2(n);       /* 2^n   */
    seq_t pow4n   = pow2(two_n);   /* 4^n   = 2^(2n) */

    /* positive part: 2^n*(2800*n+136) + 4^n*(320*n) */
    seq_t pos_term = pow2n * ( (seq_t)2800 * n + 136 )
                     + pow4n * ( (seq_t)320 * n );

    /* negative part: 4^n*976  */
    seq_t neg_term = pow4n * (seq_t)976;

    /* a(n) = 9180 + pos_term – neg_term  (pos_term ≥ neg_term for n≥1) */
    seq_t result = (seq_t)9180 + pos_term - neg_term;
    return result;
}
