/*
 * A002790  Denominators of Cauchy numbers of second type (= Bernoulli numbers
 *          B_n^{(n)}).
 * https://oeis.org/A002790
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, nice, easy
 * Formula:   a(n) = denominator of Integral_{x=0..1} x(x+1)...(x+n-1) dx =
 *            denominator of Sum_{k=0..n} |Stirling1(n,k)|/(k+1). For each
 *            prime p <= n+1, let E = floor(log_p(n+1)) and M = p^E. The code
 *            computes S = Sum_k c_k * p^E/(k+1) mod M, where c_k are the
 *            coefficients of the rising factorial mod M. The p-...
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 71, u64 n <= 335,
 *            u128 n <= 1000, big any n
 * Helpers:   a2790_inv, a2790_large, a2790_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../utils/a2790_inv.h"
#include "../utils/a2790_large.h"
#include "../utils/a2790_small.h"

seq_t a(idx_t n)
{
    idx_t lim = n + 1;
    unsigned char *comp = (unsigned char *)malloc((size_t)lim + 1);
    seq_t r = 1;
    idx_t p, q;

    memset(comp, 0, (size_t)lim + 1);
    for (p = 2; p <= lim; p++) {
        idx_t f;
        if (comp[p]) continue;
        for (q = p * p; q <= lim; q += p) comp[q] = 1;
        if (p * p > lim) {
            f = a2790_large(n, p);
        } else {
            idx_t s = 0, m = n, E = 0;
            uint64_t M = 1;
            while (m > 0) { s += m % p; m /= p; }
            if (s < p - 1) continue;
            while (M * p <= lim) { M *= p; E++; }
            f = a2790_small(n, p, E, M);
        }
        while (f-- > 0) r *= (seq_t)p;
    }
    free(comp);
    return r;
}
