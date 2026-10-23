/*
 * A027540  Second diagonal of A027537.
 * https://oeis.org/A027540
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*(2^n - n - 1)
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 27, u64 n <= 58, u128 n <= 121,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;
    seq_t power = pow2(n);                  /* 2^n */
    seq_t temp   = power - (seq_t)n - 1;    /* 2^n - n - 1 */
    return ((seq_t)n + 1) * temp;           /* (n+1)*(2^n - n - 1) */
}
