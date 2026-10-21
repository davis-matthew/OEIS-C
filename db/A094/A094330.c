/*
 * A094330  Product of next n numbers divided by n.
 * https://oeis.org/A094330
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (∏_{k=T(n-1)+1}^{T(n)} k) / n where T(m) = m(m+1)/2
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;           /* offset starts at 1; this guard is safe */

    seq_t start = ((seq_t)n * (n - 1)) / 2 + 1;   /* T(n-1)+1 */
    seq_t prod  = 1;
    seq_t rem   = (seq_t)n;                        /* remaining factor of n */

    for (idx_t i = 0; i < n; ++i)
    {
        seq_t term = start + i;                 /* current factor */
        seq_t g    = gcd(term, rem);            /* cancel common part of n */
        term /= g;
        rem  /= g;
        prod *= term;
    }

    return prod;    /* rem must now be 1, so prod = product/n */
}
