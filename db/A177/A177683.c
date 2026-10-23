/*
 * A177683  a(n) = 6*a(n-1)-8*a(n-2) for n > 4; a(0)=191, a(1)=1587,
 *          a(2)=14161, a(3)=123004, a(4)=508152.
 * https://oeis.org/A177683
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   * a(n) = 2*4^(n+5) - 2017*2^(n-1) for n > 2 * Initial values:
 *            a(0)=191, a(1)=1587, a(2)=14161
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 10, u64 n <= 26, u128 n <= 58,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    switch (n) {
        case 0ULL: return 191;
        case 1ULL: return 1587;
        case 2ULL: return 14161;
        default:
            /* 4^(n+5) == 2^(2*(n+5)) */
            seq_t term1 = pow2(2 * (n + 5));
            term1 = term1 * 2;                 /* 2 * 4^(n+5) */
            seq_t term2 = pow2(n - 1);
            term2 = term2 * 2017;              /* 2017 * 2^(n-1) */
            return term1 - term2;
    }
}
