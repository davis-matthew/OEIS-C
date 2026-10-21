/*
 * A055522  Largest area of a Pythagorean triangle with n as length of one of
 *          the three sides (in fact as a leg).
 * https://oeis.org/A055522
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2k) = k*(k+1)*(k-1); a(2k+1) = k*(k+1)*(2k+1)
 * Range:     u8 n <= 10, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n may be < 3; behaviour is irrelevant below the offset */
    if ((n & 1) == 0) {                /* even n = 2k */
        seq_t k = (seq_t)(n / 2);
        return k * (k + 1) * (k - 1);
    } else {                            /* odd n = 2k+1 */
        seq_t k = (seq_t)((n - 1) / 2);
        return k * (k + 1) * (seq_t)n;
    }
}
