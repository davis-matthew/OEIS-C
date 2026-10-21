/*
 * A090989  Number of meaningful differential operations of the n-th order on
 *          the space R^4.
 * https://oeis.org/A090989
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^((n+3)/2) for odd n, a(n) = 3 * 2^(n/2) for even n
 *            (equivalently, a(1)=4, a(2)=6, a(k+2)=2*a(k) for k≥1)
 * Range:     u8 n <= 12, u16 n <= 28, u32 n <= 60, u64 n <= 124,
 *            u128 n <= 252, big any n
 * Helpers:   power_two
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/power_two.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1.  Behaviour for n < 1 is not
       required. */
    if ((n & 1) != 0) {                 /* odd index */
        idx_t e = (n + 3) >> 1;         /* (n+3)/2 */
        return power_two(e);
    } else {                            /* even index */
        idx_t e = n >> 1;               /* n/2 */
        return (seq_t)3 * power_two(e);
    }
}
