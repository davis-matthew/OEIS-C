/*
 * A005218  Number of unlabeled reduced unit interval graphs on n nodes.
 * https://oeis.org/A005218
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   G.f.: -z + (1/4)*(1+2z-z^2)/sqrt((1+z^2)*(1-3z^2)) -
 *            (1/4)*sqrt((1-3z)/(1+z)), i.e. for n >= 2: a(n) = (R(n-1) +
 *            T(k))/2 if n = 2k+1, a(n) = R(n-1)/2 + (T(k) - T(k-1))/4 if n =
 *            2k, where R = Riordan numbers A005043, T = central trinomial
 *            coefficients A002426
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 27, u64 n <= 48, u128 n <= 89,
 *            big any n
 * Helpers:   add2, riordan_add2, riordan_rec, seq_is_fixed_width,
 *            trinom_central_add, trinom_central_rec
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_is_fixed_width.h"
#include "../utils/add2.h"
#include "../utils/riordan_add2.h"
#include "../utils/riordan_rec.h"
#include "../utils/trinom_central_add.h"
#include "../utils/trinom_central_rec.h"

seq_t a(idx_t n) {
    idx_t m, k;
    bool fixed;
    seq_t h;
    if (n <= 1) return 0;
    m = n - 1;
    k = n / 2;
    fixed = seq_is_fixed_width();
    if (n % 2 == 1) {
        h = fixed ? trinom_central_add(k) : trinom_central_rec(k);
    } else {
        seq_t tk = fixed ? trinom_central_add(k) : trinom_central_rec(k);
        seq_t tk1 = fixed ? trinom_central_add(k - 1) : trinom_central_rec(k - 1);
        h = (tk - tk1) / 2;
    }
    if (fixed) {
        seq_t lo, hi, x = 1;
        int w = 0;
        while (x != 0) { x <<= 1; w++; }
        riordan_add2(m, &lo, &hi);
        add2(&lo, &hi, h, 0);
        return (lo >> 1) | (hi << (w - 1));
    } else {
        seq_t r = riordan_rec(m);
        return (r + h) / 2;
    }
}
