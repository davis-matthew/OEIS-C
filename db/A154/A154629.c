/*
 * A154629  Period 9: repeat [9, 3, 1, 3, 3, 1, 3, 9, 1].
 * https://oeis.org/A154629
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = gcd((n+1)^2 - 1, 9)
 * Range:     u8 n <= 15, u16 n <= 275, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* compute (n+1)^2 - 1 */
    seq_t m    = (seq_t)n + (seq_t)1;
    seq_t val  = m * m;
    val        = val - (seq_t)1;
    /* a(n) is the gcd of this value and 9 */
    return gcd(val, (seq_t)9);
}
