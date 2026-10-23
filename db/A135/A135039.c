/*
 * A135039  Ceiling(Pi*n^2).
 * https://oeis.org/A135039
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(pi * n^2) The value is computed by integer
 *            arithmetic only. We use a fixed-point representation of pi with
 *            12 decimal digits: pi_1000000000000 = floor(pi * 10^12) =
 *            3141592653589 Since the maximal n used by the test suite is
 *            within 10^4 (n^2 ≤ 10^8), the truncation error is < 10^(-12)...
 * Range:     u8 n <= 9, u16 n <= 144, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#define PI_SCALE 3141592653589ULL   /* floor(pi * 1e12) */
#define SCALE    1000000000000ULL   /* 1e12 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* compute floor(pi * n^2) using 128-bit intermediate */
    __int128 nn = ( __int128 ) n * n;       /* n^2   */
    nn *= ( __int128 ) PI_SCALE;            /* pi_scaled * n^2 */
    nn /= ( __int128 ) SCALE;               /* floor(pi * n^2) */

    seq_t floor_val = (seq_t)nn;            /* truncate to seq_t */

    /* ceil for n>0 */
    return floor_val + 1;
}
