/*
 * A034583  Dimension of an irreducible R-module for Clifford algebra Cl_n.
 * https://oeis.org/A034583
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^e with e = 4*(n/8) + {0,1,2,2,3,3,3,3}[n%8]
 * Range:     u8 n <= 15, u16 n <= 31, u32 n <= 63, u64 n <= 127,
 *            u128 n <= 255, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    idx_t base = n / 8;          /* 4*k part of the exponent   */
    idx_t rem  = n & 7;          /* n mod 8                     */
    idx_t exp  = base << 2;      /* 4 * base = 4*(n/8)         */

    /* Add the appropriate offset according to n mod 8 */
    switch (rem) {
        case 0: break;           /* +0 */
        case 1: exp += 1; break; /* +1 */
        case 2: exp += 2; break; /* +2 */
        case 3: exp += 2; break; /* +2 */
        case 4: exp += 3; break; /* +3 */
        case 5: exp += 3; break; /* +3 */
        case 6: exp += 3; break; /* +3 */
        case 7: exp += 3; break; /* +3 */
    }

    return pow2(exp);
}
