/*
 * A002553  Coefficients for numerical differentiation.
 * https://oeis.org/A002553
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = denom(A001818(n)*(-1)^(n-1)/(2^(2*n)*(2*n+1)!)),
 *            A001818(n) = ((2n-1)!!)^2, computed prime by prime: v_p(a(n)) =
 *            max(0, v_p((2n+1)!) + 2n*[p=2] - 2*v_p((2n-1)!!)), with
 *            v_p((2n-1)!!) = v_p((2n)!) - v_p(n!) for odd p and 0 for p = 2
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 16,
 *            big any n
 * Helpers:   a002553_pw, a002553_vf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/a002553_pw.h"
#include "../utils/a002553_vf.h"

seq_t a(idx_t n) {
    unsigned long long M = 2ULL * (unsigned long long)n + 1ULL;
    unsigned char *comp = (unsigned char *)calloc((size_t)(M + 1), 1);
    seq_t r = (seq_t)1;
    unsigned long long p, q;
    for (p = 2; p <= M; p++) {
        long long e;
        if (comp[p]) continue;
        for (q = p * p; q <= M; q += p) comp[q] = 1;
        if (p == 2) {
            e = a002553_vf(M, 2) + 2LL * (long long)n;
        } else {
            long long vdf = a002553_vf(M - 1, p) - a002553_vf(n, p);
            e = a002553_vf(M, p) - 2 * vdf;
        }
        if (e > 0) r *= a002553_pw((seq_t)p, e);
    }
    free(comp);
    return r;
}
