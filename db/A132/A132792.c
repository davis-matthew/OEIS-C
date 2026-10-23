/*
 * A132792  The infinitesimal Lah matrix: generator of unsigned A111596.
 * https://oeis.org/A132792
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = 0 unless 17 + 8n is a perfect square s² with odd s, * in
 *            which case a(n) = ((s-5)/2) * ((s-5)/2 + 1). * Otherwise a(n) =
 *            0
 * Range:     u8 n <= 5, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Compute d = 17 + 8*n in the seq_t type. */
    seq_t d = (seq_t)17 + ((seq_t)8 * (seq_t)n);

    /* Integer sqrt of d. */
    seq_t s = floor_sqrt(d);

    /* Must be a perfect square. */
    if (s * s != d)
        return 0;

    /* Must be odd. */
    if ((s & 1) == 0)
        return 0;

    /* Compute k = (s - 5) / 2.  s >= 5 because smallest odd square root
     * giving a non‑zero a(n) is 7.  For s = 5, k = 0 which yields a(n)=0. */
    seq_t k = (s >= 5) ? ((s - 5) / 2) : 0;

    /* If k == 0, a(n) is 0. */
    if (k == 0)
        return 0;

    /* a(n) = k * (k + 1). */
    return k * (k + 1);
}
