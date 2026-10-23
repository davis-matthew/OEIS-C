/*
 * A177422  a(n) = 6*a(n-1)-8*a(n-2) for n > 10; a(0)=221, a(1)=1938,
 *          a(2)=8673, a(3)=73729, a(4)=589855, a(5)=7561526, a(6)=34593784,
 *          a(7)=218391421, a(8)=2116566392, a(9)=8522858480,
 *          a(10)=34225586144.
 * https://oeis.org/A177422
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*4^(n+7) - 4192257*2^(n-5) for n > 8 Initial values
 *            (from the OEIS entry): a(0)=221, a(1)=1938, a(2)=8673,
 *            a(3)=73729, a(4)=589855, a(5)=7561526, a(6)=34593784,
 *            a(7)=218391421, a(8)=2116566392
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 8, u64 n <= 24, u128 n <= 56,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n < 9) {
        /* First 9 terms – stored as a small seed array, not a lookup table of the whole sequence */
        static const seq_t init[9] = {
            221ULL, 1938ULL, 8673ULL, 73729ULL,
            589855ULL, 7561526ULL, 34593784ULL,
            218391421ULL, 2116566392ULL
        };
        return init[n];
    }

    /* For n ≥ 9 use the closed‑form expression.
       pow2 is provided by the utilities and returns 2^exp as seq_t (handles
       arbitrary‑size exponents). */
    seq_t term1 = (seq_t)2 * pow2((idx_t)2 * (n + 7));   /* 2 * 4^(n+7) = 2 * 2^(2(n+7)) */
    seq_t term2 = (seq_t)4192257ULL * pow2(n - 5);        /* 4192257 * 2^(n-5)     */
    return term1 - term2;
}
