/*
 * A005946  Number of n-step mappings with 5 inputs.
 * https://oeis.org/A005946
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   G.f.: x*(24*x^3+108*x^2+47*x+1)/(1-x)^5, i.e. a(n) = C(n+3,4) +
 *            47*C(n+2,4) + 108*C(n+1,4) + 24*C(n,4)
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 155, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a005946_c4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a005946_c4.h"

seq_t a(idx_t n)
{
    seq_t c1 = a005946_c4(n + 3);
    seq_t c2 = a005946_c4(n + 2);
    seq_t c3 = a005946_c4(n + 1);
    seq_t c4 = a005946_c4(n);
    return c1 + c2 * 47 + c3 * 108 + c4 * 24;
}
