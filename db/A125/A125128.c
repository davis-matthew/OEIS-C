/*
 * A125128  a(n) = 2^(n+1) - n - 2, or partial sums of main diagonal of array
 *          A125127 of k-step Lucas numbers.
 * https://oeis.org/A125128
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^(n+1) - n - 2
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n) {
    if (n == 0) {
        return 0; /* undefined for n=0, return 0 as fallback */
    }
    seq_t result = pow2(n + 1);     /* 2^(n+1) */
    result -= ((seq_t)n) + 2;       /* subtract n and 2 */
    return result;
}
