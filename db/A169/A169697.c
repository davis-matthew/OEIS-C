/*
 * A169697  The rows of A169689 converge to 4 times this sequence.
 * https://oeis.org/A169697
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (3^(m+1)-2^(m+1))*3^popcount(t) where n = 2^m*(2t+1)
 * Range:     u8 n <= 31, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow2.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    if (n == 0)            /* safety for n < 1, not used in OEIS */
        return 0;

    /* find exponent m of the largest power of two dividing n */
    idx_t m = 0;
    idx_t tmp = n;
    while ((tmp & 1ULL) == 0) {
        tmp >>= 1;
        ++m;
    }

    /* odd part = 2t+1 */
    idx_t odd = tmp;
    idx_t t   = (odd - 1) >> 1;

    /* popcount of t (number of 1-bits) */
    idx_t wt = __builtin_popcountll((unsigned long long)t);

    seq_t part1 = pow3(m + 1) - pow2(m + 1);
    seq_t part2 = pow3(wt);

    return part1 * part2;
}
