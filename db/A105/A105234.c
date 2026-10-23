/*
 * A105234  Central column of a Moebius-binomial triangle.
 * https://oeis.org/A105234
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binomial(abs(mu(2n)), abs(mu(n)))
 * Range:     u8 n <= 9555, u16 n <= 9555, u32 n <= 9555, u64 n <= 9555,
 *            u128 n <= 9555, big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent: a(n) = 0 if n is an even square‑free integer; otherwise 1. */

/* External utilities available in the build environment */
#include <stdbool.h>
#include "../utils/is_squarefree.h"

seq_t a(idx_t n)
{
    /* Handle the OEIS offset: a(0)=1.  In the Möbius convention 0 is not
       square‑free, so we special‑case it. */
    if (n == 0)
        return (seq_t)1;

    /* n even and square‑free → binomial(0,1)=0; otherwise → 1. */
    if ( (n & 1ULL) == 0 && is_squarefree(n) )
        return (seq_t)0;

    return (seq_t)1;
}
