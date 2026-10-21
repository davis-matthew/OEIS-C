/*
 * A081107  7th binomial transform of (1,1,0,0,0,0,...).
 * https://oeis.org/A081107
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n + 7) * 7^(n-1) for n >= 1, a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>        /* for uint64_t, etc.  (seq_t and idx_t are supplied by the build) */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* compute 7^(n-1) */
    seq_t pow = (seq_t)1;
    for (idx_t i = 1; i < n; ++i)
        pow *= (seq_t)7;

    /* multiply by (n + 7) */
    seq_t mult = ((seq_t)n) + (seq_t)7;
    return pow * mult;
}
