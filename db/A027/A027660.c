/*
 * A027660  a(n) = C(n+2, 2) + C(n+2, 3) + C(n+2, 4) + C(n+2, 5).
 * https://oeis.org/A027660
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+3,5) + binomial(n+3,3)
 * Range:     u8 n <= 5, u16 n <= 16, u32 n <= 162, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   _seq_binom_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/_seq_binom_small.h"

seq_t a(idx_t n)
{
    /* offset is 0, so use n directly */
    idx_t m = n + 3;                    /* we need C(m,5) + C(m,3) */

    seq_t term5 = _seq_binom_small(m, 5);
    seq_t term3 = _seq_binom_small(m, 3);

    return term5 + term3;
}
