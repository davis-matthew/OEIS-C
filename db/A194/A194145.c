/*
 * A194145  Beatty sequence for -1+sqrt(6), a(n) = floor(n*(-1+sqrt(6)));
 *          complement of A194146.
 * https://oeis.org/A194145
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*(sqrt(6)-1)) = floor(n*sqrt(6)) - n
 * Range:     u8 n <= 3, u16 n <= 52, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses integer arithmetic only, relying on the provided
   integer square‑root helper floor_sqrt. */
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Convert n to seq_t for arithmetic in the target integer width. */
    seq_t nn = (seq_t)n;

    /* Compute floor(n*sqrt(6)) as floor(sqrt(6*n^2)). */
    seq_t sq = nn * nn * 6;
    seq_t fns = floor_sqrt(sq);

    /* Subtract n to obtain floor(n*(sqrt(6)-1)). */
    return fns - nn;
}
