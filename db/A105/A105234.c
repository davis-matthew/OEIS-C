/*
 * A105234  Central column of a Moebius-binomial triangle.
 * https://oeis.org/A105234
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binomial(abs(mu(2n)), abs(mu(n)))
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 117 terms through n = 9555 (2026-11-01)
 * Helpers:   is_squarefree_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent: a(n) = 0 if n is an even square‑free integer; otherwise 1. */

/* External utilities available in the build environment */
#include <stdbool.h>
#include "../utils/is_squarefree_idx.h"

seq_t a(idx_t n)
{
    /* Handle the OEIS offset: a(0)=1.  In the Möbius convention 0 is not
       square‑free, so we special‑case it. */
    if (n == 0)
        return (seq_t)1;

    /* n even and square‑free → binomial(0,1)=0; otherwise → 1. */
    if ( (n & 1ULL) == 0 && is_squarefree_idx(n) )
        return (seq_t)0;

    return (seq_t)1;
}
