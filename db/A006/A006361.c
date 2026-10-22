/*
 * A006361  Antichains (or order ideals) in the poset 2*2*4*n or size of the
 *          distributive lattice J(2*2*4*n).
 * https://oeis.org/A006361
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{j=0..10} c(j)*binomial(n+16-j,16), where c =
 *            [1,88,1841,13812,44050,64374,44050,13812,1841,88,1]; this is the
 *            expansion of the G.f.
 *            (x^10+88*x^9+1841*x^8+13812*x^7+44050*x^6+64374*x^5+44050*x^4+13812*x^3+1841*x^2+88*x+1)/(1-x)^17.
 *            The order polynomial of the 16-element poset 2*2*4 has...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

static const int a006361_c[11] = {
    1, 88, 1841, 13812, 44050, 64374, 44050, 13812, 1841, 88, 1
};

seq_t a(idx_t n)
{
    seq_t result = 0;
    idx_t j;
    for (j = 0; j <= 10; j++) {
        seq_t t = binom(n + 16 - j, 16);
        t *= a006361_c[j];
        result += t;
    }
    return result;
}
