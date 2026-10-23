/*
 * A151791  a(1)=1; for n > 1, a(n) = 7*6^(wt(n-1)-1).
 * https://oeis.org/A151791
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(1)=1; for n>1, a(n) = 7 * 6^(popcount(n-1)-1)
 * Range:     u8 n <= 15, u16 n <= 127, u32 n <= 8191, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   seq_mul, seq_one, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_one.h"
#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    if (n == 1) return seq_one();

    /* popcount of n-1 */
    unsigned int bits = __builtin_popcountll((uint64_t)(n - 1));
    idx_t exp = bits - 1;           /* exponent for 6  (at least 0) */

    seq_t tmp = seq_pow(6, exp);
    return seq_mul(tmp, 7);
}
