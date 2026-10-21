/*
 * A092266  Expansion of (1+4x)/AGM(1+4x,1-4x) where AGM denotes the
 *          arithmetic-geometric mean.
 * https://oeis.org/A092266
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (C(2*floor(n/2), floor(n/2)))^2 * (4 if n is odd, 1
 *            otherwise)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 18, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t k = n >> 1;          /* k = floor(n/2) */
    seq_t res = 1;             /* res will hold C(2k, k) */

    for (idx_t i = 1; i <= k; ++i) {
        seq_t num = (seq_t)(k + i);
        res = res * num / (seq_t)i;   /* maintain integer division */
    }

    if (n & 1) {                  /* odd n: multiply by 4 */
        return res * res * (seq_t)4;
    } else {                      /* even n */
        return res * res;
    }
}
