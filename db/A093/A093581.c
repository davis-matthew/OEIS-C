/*
 * A093581  Numerators of odd moments in the distribution of chord lengths for
 *          picked at random on the circumference of a unit circle.
 * https://oeis.org/A093581
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = 4*A061549(n-1) * A061549(k) = 2^{4k - popcount(k)} *
 *            Hence a(n) = 2^{4n - 2 - popcount(n-1)}
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t k = n - 1u;          /* n >= 1 */
    /* popcount of k */
    uint64_t pc = 0;
    idx_t t = k;
    while (t != 0u) {
        pc++;
        t &= t - 1u;          /* drop lowest set bit */
    }
    idx_t e = 4 * n - 2 - pc;  /* exponent for 2^e */
    seq_t val = (seq_t)1;
    val <<= e;
    return val;
}
