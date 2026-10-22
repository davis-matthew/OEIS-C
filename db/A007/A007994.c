/*
 * A007994  Poincaré series (or Poincare series) of Lie algebra associated
 *          with a certain braid group.
 * https://oeis.org/A007994
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   G.f.:
 *            x*(8*x^9-54*x^8+144*x^7-178*x^6+60*x^5+102*x^4-137*x^3+70*x^2-18*x+4)/(1-x)^10,
 *            i.e. a(n) = Sum_{j=0..9} p_j * binomial(n+8-j, 9) with
 *            (p_0..p_9) = (4,-18,70,-137,102,60,-178,144,-54,8)
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 43, u64 n <= 568, u128 n <= 1000,
 *            big any n
 * Helpers:   c9
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/c9.h"

seq_t a(idx_t n) {
    seq_t pos, neg;
    pos = (seq_t)4 * c9(n + 8) + (seq_t)70 * c9(n + 6) + (seq_t)102 * c9(n + 4)
        + (seq_t)60 * c9(n + 3) + (seq_t)144 * c9(n + 1) + (seq_t)8 * c9(n - 1);
    neg = (seq_t)18 * c9(n + 7) + (seq_t)137 * c9(n + 5) + (seq_t)178 * c9(n + 2)
        + (seq_t)54 * c9(n);
    return pos - neg;
}
