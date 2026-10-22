/*
 * A005569  Number of walks on square lattice.
 * https://oeis.org/A005569
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, walk, easy
 * Formula:   a(n) = 2*n*(5*n+7)*C(n)*C(n+1)/((n+2)*(n+3)), where C(k) =
 *            binomial(2k,k)/(k+1) are the Catalan numbers (closed form of
 *            a(n)(n-1)(n+2)(n+3)(5n+2) = a(n-1)4n(2n-1)(2n+1)(5n+7), a(1) =
 *            4)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   g64, gseq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/g64.h"
#include "../utils/gseq.h"

/* Catalan number C(m), computed multiplicatively with reduction */
static seq_t catalan(idx_t m) {
    seq_t c = 1;
    idx_t k;
    for (k = 1; k <= m; k++) {
        uint64_t num = 2 * (2 * (uint64_t)k - 1);
        uint64_t den = (uint64_t)k + 1;
        uint64_t g = g64(num, den);
        num /= g; den /= g;
        /* den now divides c */
        if ((seq_t)den != 0) c /= (seq_t)den;
        c *= (seq_t)num;
    }
    return c;
}

seq_t a(idx_t n) {
    seq_t cn = catalan(n);
    seq_t cn1;
    {
        uint64_t num = 2 * (2 * (uint64_t)n + 1);
        uint64_t den = (uint64_t)n + 2;
        uint64_t g = g64(num, den);
        num /= g; den /= g;
        cn1 = cn;
        if ((seq_t)den != 0) cn1 /= (seq_t)den;
        cn1 *= (seq_t)num;
    }

    uint64_t nn = 2 * (uint64_t)n * (5 * (uint64_t)n + 7);
    uint64_t d = ((uint64_t)n + 2) * ((uint64_t)n + 3);
    uint64_t g = g64(nn, d);
    nn /= g; d /= g;

    /* d divides cn*cn1 */
    seq_t g2 = gseq(cn, (seq_t)d);
    if (g2 != 0) {
        cn /= g2;
        uint64_t g2u = (uint64_t)(unsigned long long)g2;
        if (g2u != 0 && d % g2u == 0) d /= g2u;
    }
    if ((seq_t)d != 0) cn1 /= (seq_t)d;

    return cn * cn1 * (seq_t)nn;
}
