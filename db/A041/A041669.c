/*
 * A041669  Denominators of continued fraction convergents to sqrt(353).
 * https://oeis.org/A041669
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 14, u32 n <= 29, u64 n <= 59, u128 n <= 115,
 *            big any n
 * Helpers:   get_partial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Denominators of continued fraction convergents to sqrt(353)
   Recurrence used (for reference): a(n)=142528*a(n-15)+a(n-30) for n>29
   Computed by the standard recurrence for continued‑fraction convergents. */

#include <stddef.h>   /* for size_t, if needed */
#include "../utils/get_partial.h"

/* Compute a(n) = denominator of the n-th convergent of sqrt(353). */
seq_t a(idx_t n)
{
    seq_t prev2 = (seq_t)1;   /* q_{-2} */
    seq_t prev1 = (seq_t)0;   /* q_{-1} */
    seq_t q = 0;

    for (idx_t k = 0; k <= n; ++k) {
        seq_t ak = get_partial(k);
        q = ak * prev1 + prev2;
        prev2 = prev1;
        prev1 = q;
    }
    return q;
}
