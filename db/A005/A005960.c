/*
 * A005960  Number of acyclic disubstituted alkanes with n carbon atoms and
 *          distinct substituents.
 * https://oeis.org/A005960
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   G.f. 1/(1-x*B(x)^2) = Sum_{n>=0} a(n) x^n, where B(x) is the
 *            g.f. of A000625: B(x) = 1 + x*(B(x)^3 + 2*B(x^3))/3, so b(0) =
 *            1, b(m) = ([x^(m-1)] B^3 + 2*b((m-1)/3) if 3|(m-1)) / 3. Then
 *            a(0) = 1, a(m) = Sum_{k=0..m-1} [x^k]B^2 * a(m-1-k)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 20, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   alloc_zero, grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"

static seq_t *bb = NULL, *s2 = NULL, *t3 = NULL, *av = NULL;
static idx_t done = 0, cap = 0;

static void extend_to(idx_t N) {
    if (N > cap) {
        idx_t nc = cap ? cap : 64;
        while (nc < N) nc *= 2;
        if (!bb) {
            bb = alloc_zero(nc); s2 = alloc_zero(nc);
            t3 = alloc_zero(nc); av = alloc_zero(nc);
        } else {
            bb = grow_zero(bb, cap, nc); s2 = grow_zero(s2, cap, nc);
            t3 = grow_zero(t3, cap, nc); av = grow_zero(av, cap, nc);
        }
        cap = nc;
    }
    for (idx_t m = done; m < N; ++m) {
        if (m == 0) {
            bb[0] = 1; s2[0] = 1; t3[0] = 1; av[0] = 1;
            continue;
        }
        seq_t v = t3[m - 1];
        if ((m - 1) % 3 == 0) v += (seq_t)2 * bb[(m - 1) / 3];
        bb[m] = v / 3;

        seq_t s = 0;
        for (idx_t i = 0; i <= m; ++i) s += bb[i] * bb[m - i];
        s2[m] = s;

        seq_t t = 0;
        for (idx_t i = 0; i <= m; ++i) t += bb[i] * s2[m - i];
        t3[m] = t;

        seq_t r = 0;
        for (idx_t k = 0; k < m; ++k) r += s2[k] * av[m - 1 - k];
        av[m] = r;
    }
    if (N > done) done = N;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    extend_to(n + 1);
    return av[n];
}
