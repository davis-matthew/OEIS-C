/*
 * A050488  a(n) = 3*(2^n-1) - 2*n.
 * https://oeis.org/A050488
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*(2^n-1) - 2*n
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   pow2_fast
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2_fast.h"

seq_t a(idx_t n)
{
    seq_t pow = pow2_fast(n);          /* 2^n */
    seq_t val = pow - 1;               /* 2^n - 1 */
    val *= 3;                          /* 3*(2^n - 1) */
    val -= ((seq_t)2) * (seq_t)n;      /* - 2*n */
    return val;
}
