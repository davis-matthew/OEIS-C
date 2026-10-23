/*
 * A173033  Second diagonal under the main diagonal in A172119 written in a
 *          square (see comment).
 * https://oeis.org/A173033
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 1, a(n) = 2^(n+2) - 4 for n ≥ 1
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }
    /* compute 2^(n+2) */
    seq_t val = big_pow((seq_t)2, (seq_t)(n + 2));
    /* subtract 4 */
    return val - (seq_t)4;
}
