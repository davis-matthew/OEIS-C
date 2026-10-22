/*
 * A006434  Number of tree-rooted toroidal maps with 2 faces and n vertices
 *          and without separating loops or isthmuses.
 * https://oeis.org/A006434
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 2*(2*C(n+8,9) + 40*C(n+7,9) + 105*C(n+6,9) + 22*C(n+5,9)
 *            - 13*C(n+4,9) + 8*C(n+3,9) + C(n+2,9)), from g.f.
 *            2*x*(x^6+8*x^5-13*x^4+22*x^3+105*x^2+40*x+2) / (1-x)^10
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   a006434_binom, a006434_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a006434_binom.h"
#include "../utils/a006434_gcd.h"

seq_t a(idx_t n) {
    seq_t c0 = a006434_binom(n + 8, 9);
    seq_t c1 = a006434_binom(n + 7, 9);
    seq_t c2 = a006434_binom(n + 6, 9);
    seq_t c3 = a006434_binom(n + 5, 9);
    seq_t c4 = a006434_binom(n + 4, 9);
    seq_t c5 = a006434_binom(n + 3, 9);
    seq_t c6 = a006434_binom(n + 2, 9);

    seq_t result = c0 * 2 + c1 * 40 + c2 * 105 + c3 * 22 + c5 * 8 + c6;
    result = result - c4 * 13;
    return result * 2;
}
