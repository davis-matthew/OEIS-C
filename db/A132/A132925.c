/*
 * A132925  a(n) = 2^n - 1 + n*(n-1)/2.
 * https://oeis.org/A132925
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n - 1 + n*(n-1)/2
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t power = pow2(n);                       /* 2^n */
    seq_t tri   = ((seq_t)n * ((seq_t)n - (seq_t)1)) / (seq_t)2; /* n*(n-1)/2 */
    return power - (seq_t)1 + tri;              /* 2^n - 1 + n*(n-1)/2 */
}
