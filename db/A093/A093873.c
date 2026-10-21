/*
 * A093873  Numerators in Kepler's tree of harmonic fractions.
 * https://oeis.org/A093873
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac, look, hear
 * Formula:   a(1) = 1. For n ≥ 2 let p = ⌊n/2⌋. If n is even: a(n) = a(p); If
 *            n is odd : a(n) = b(p), where b(1) = 1 and for every node: b(2p)
 *            = b(2p+1) = a(p) + b(p). This is equivalent to following the
 *            binary path from the root of Kepler's tree of harmonic
 *            fractions, keeping the current numerator (a) and de...
 * Range:     u8 n <= 9564, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety, though the sequence is defined for n ≥ 1 */

    seq_t a_val = 1;   /* current numerator   */
    seq_t b_val = 1;   /* current denominator */

    /* Find floor(log2(n)) */
    idx_t msb = 0;
    idx_t tmp = n;
    while (tmp >>= 1)
        ++msb;

    /* Process bits from the second most significant down to 0. */
    for (idx_t k = msb; k-- > 0; ) {
        idx_t bit = (n >> k) & 1ULL;
        seq_t old_a = a_val;
        seq_t old_b = b_val;
        if (bit)          /* right child: numerator becomes old denominator */
            a_val = old_b;
        /* denominator for either child is old_a + old_b */
        b_val = old_a + old_b;
    }
    return a_val;
}
