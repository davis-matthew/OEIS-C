/*
 * A211415  Number of ordered triples (w,x,y) with all terms in
 *          {-n,...,0,...,n} and w^2 + x*y = 1.
 * https://oeis.org/A211415
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered triples (w,x,y) with all terms in
 *            {-n,…,0,…,n} and w^2 + x*y = 1
 * Range:     u8 n <= 14, u16 n <= 424, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_pairs_abs
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/count_pairs_abs.h"

seq_t a(idx_t n)
{
    seq_t result = 0;

    /* Contribution from w = 0 : d = 1 - 0^2 = 1. */
    result += count_pairs_abs(1, n);

    /* Contributions from w = 1 … n.  For w > 0, |1 - w^2| = w^2 - 1. */
    for (idx_t w = 1; w <= n; ++w) {
        seq_t wsq = (seq_t)w * w;          /* w^2 fits in 64 bits. */
        seq_t m = (w == 1) ? 0 : wsq - 1; /* |1 - w^2| for w > 0. */
        seq_t cp = count_pairs_abs(m, n);
        result += 2 * cp;                 /* account for w = ±w. */
    }

    return result;
}
