/*
 * A059010  Natural numbers having an even number of nonleading zeros in their
 *          binary expansion.
 * https://oeis.org/A059010
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base, nice
 * Formula:   * a(0) = 1 * a(2n) = 6*n + 1 – a(n) * a(2n+1) = a(n) + 2*n + 2 *
 *            where n = ⌊index/2⌋ for the odd case. * (This follows the
 *            recurrence given in the OEIS entry.)
 * Range:     u8 n <= 127, u16 n <= 9589, u32 n <= 9589, u64 n <= 9589,
 *            u128 n <= 9589, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    if ((n & 1ull) == 0)            /* even index */
    {
        idx_t m = n >> 1;           /* n = 2*m */
        seq_t t = (seq_t)6 * (seq_t)m + 1;   /* 6*m + 1 */
        t -= a(m);                  /* 6*m + 1 - a(m) */
        return t;
    }
    else                            /* odd index */
    {
        idx_t m = n >> 1;           /* n = 2*m + 1 */
        return a(m) + (seq_t)2 * (seq_t)m + 2; /* a(m) + 2*m + 2 */
    }
}
