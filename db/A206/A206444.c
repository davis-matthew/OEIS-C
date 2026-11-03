/*
 * A206444  Least n such that L(n)<-1 and L(n)<L(n-1), where L(k) means the
 *          least root of the polynomial p(k,x) defined at A206284, and
 *          a(1)=13.
 * https://oeis.org/A206444
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1)+1, a(1)=13
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Validated: partial, pass, 10 terms through n = 10 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* not used, but safe */
    /* compute 4^(n-1) */
    seq_t pow4 = ipow_idx((seq_t)4, n - 1);

    /* a(n) = 13 * 4^(n-1) + (4^(n-1) - 1) / 3 */
    seq_t result = (seq_t)13 * pow4;
    result += (pow4 - (seq_t)1) / (seq_t)3;
    return result;
}
