/*
 * A132681  Infinitesimal generator matrix for a diagonally-shifted Pascal
 *          matrix, binomial(n+m,k+m), for m=1, related to Laguerre(n,x,m).
 * https://oeis.org/A132681
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = (√(8n+17) - 1)/2 if 8n+17 is a perfect square (√ odd), 0
 *            otherwise (OEIS A132681)
 * Range:     u8 n <= 5, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    // 8*n + 17
    seq_t D = ((seq_t)8 * n) + 17;
    seq_t s = floor_sqrt(D);
    if (s * s != D)          // not a perfect square
        return 0;
    if ((s & 1) == 0)        // square root must be odd
        return 0;
    return (s - 1) / 2;      // value at the special index
}
