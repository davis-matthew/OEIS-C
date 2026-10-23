/*
 * A153398  G.f.: A(x) = F(x*G(x)^2) where F(x) = G(x/F(x)) = 1 + x*F(x)^3 is
 *          the g.f. of A001764 and G(x) = F(x*G(x)) = 1 + x*G(x)^4 is the
 *          g.f. of A002293.
 * https://oeis.org/A153398
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A153398
 *  Formula: a(0) = 1
 *           a(n) = Σ_{k=1..n} C(3k+1,k)/(3k+1) * C(4n-2k,n-k) * k/(2n-k)   (n ≥ 1)
 *
 *  The binomial coefficient C(a,b) is computed with the supplied utility
 *  function binom(a,b).  All intermediate arithmetic is carried out in
 *  the exact sequence type seq_t, which may be an unsigned integer type
 *  or an arbitrary‑precision integer class.
 *  The function works for any n ≥ 0 and for any call order.  No
 *  static caching is used; each call recomputes the required sum.
 */

#include <stddef.h>            /* for size_t */
#include "../utils/binom.h"    /* seq_t binom(idx_t a, idx_t b) */

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Handle the offset. */
    if (n == 0)
        return (seq_t)1;

    seq_t sum = (seq_t)0;

    for (idx_t k = 1; k <= n; ++k)
    {
        /* Compute B = C(3k+1,k)/(3k+1) */
        seq_t B = binom(3 * k + 1, k);
        B = B / (seq_t)(3 * k + 1);        /* exact division */

        /* Compute D = C(4n-2k,n-k) */
        seq_t D = binom(4 * n - 2 * k, n - k);

        /* term = B * D * k / (2n-k) */
        seq_t term = B;
        term = term * D;
        term = term * (seq_t)k;
        term = term / (seq_t)(2 * n - k);    /* exact division */

        sum += term;
    }

    return sum;
}
