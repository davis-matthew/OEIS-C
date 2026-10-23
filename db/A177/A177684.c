/*
 * A177684  a(n) = 6*a(n-1)-8*a(n-2)-3 for n > 4; a(0)=633, a(1)=4809,
 *          a(2)=46041, a(3)=277767, a(4)=1079823.
 * https://oeis.org/A177684
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 9, u64 n <= 25, u128 n <= 57,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * A177684: a(n) = 4^(n+6) + 1953*2^n - 1 for n > 2;
 * a(0) = 633, a(1) = 4809, a(2) = 46041
 */
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    switch (n) {
        case 0: return 633;
        case 1: return 4809;
        case 2: return 46041;
        default:
            /* 4^(n+6) == 2^(2*(n+6)) */
            seq_t term1 = pow2(2 * (n + 6));
            seq_t term2 = 1953 * pow2(n);
            return term1 + term2 - 1;
    }
}
