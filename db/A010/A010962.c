/*
 * A010962  Binomial coefficient C(46,n).
 * https://oeis.org/A010962
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = C(46,n) for 0 ≤ n ≤ 46; a(n) = 0 for n > 46
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Return 0 for indices beyond the 46th row */
    if (n > 46ULL)
        return (seq_t)0;

    /* Compute the binomial coefficient C(46,n) */
    return binomial(46ULL, n);
}
