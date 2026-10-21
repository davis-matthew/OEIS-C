/*
 * A058084  Smallest m such that binomial(m,k) = n for some k.
 * https://oeis.org/A058084
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nice, nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 6871, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A058084: a(n) is the smallest row index m (starting at 0) for which
 * there exists a k such that binomial(m, k) = n.  The sequence is offset by
 * 1, so a(1) = 0, a(2) = 2, etc.  The function returns the value in type
 * seq_t; the index n is of type idx_t. */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* n == 1 occurs first in row 0 (binomial(0,0) = 1). */
    if (n == 1)
        return (seq_t)0;

    /* For any n >= 2, binomial(n,1) = n guarantees an answer at m = n.
     * We search upward from m = 2 until the match is found. */
    for (idx_t m = 2; ; ++m)
    {
        /* Only need to scan k up to the middle due to symmetry. */
        idx_t limit = m >> 1;   /* m / 2 */
        for (idx_t k = 1; k <= limit; ++k)
        {
            seq_t val = binomial(m, k);
            if (val == (seq_t)n)
                return (seq_t)m;          /* found the smallest m */
            if (val > (seq_t)n)
                break;                     /* binomial values increase to the center */
        }
    }
}
