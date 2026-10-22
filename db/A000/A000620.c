/*
 * A000620  Number of monosubstituted alkanes C(n-1)H(2n-1)-X with n-1 carbon
 *          atoms that are stereoisomers.
 * https://oeis.org/A000620
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = T(n-1) - N(n-1), where T(x) = Sum T(k) x^k = 1 +
 *            x*(T(x)^3 + 2*T(x^3))/3 counts alkyl radicals with stereoisomers
 *            counted separately (A000625), and N(x) = Sum N(k) x^k = 1 +
 *            x*N(x)*N(x^2) counts radicals that have no stereocenter
 *            (A000621); T(0) = N(0) = 1
 * Range:     u8 n <= 9, u16 n <= 14, u32 n <= 24, u64 n <= 43, u128 n <= 81,
 *            big any n
 * Helpers:   a306_grow, div3, fixed_w
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/a306_grow.h"
#include "../utils/div3.h"
#include "../utils/fixed_w.h"

static seq_t *Tt = 0, *S2 = 0, *Nn = 0;
static idx_t cnt = 0, cap = 0;

static void ensure(idx_t m) {
    idx_t k, i;
    if (m < cnt) return;
    if (m + 1 > cap) {
        idx_t nc = cap ? cap : 64;
        while (nc < m + 1) nc *= 2;
        Tt = a306_grow(Tt, cap, nc);
        S2 = a306_grow(S2, cap, nc);
        Nn = a306_grow(Nn, cap, nc);
        cap = nc;
    }
    if (cnt == 0) {
        Tt[0] = 1; S2[0] = 1; Nn[0] = 1;
        cnt = 1;
    }
    for (k = cnt; k <= m; k++) {
        seq_t s = 0, sn = 0, s2 = 0;
        for (i = 0; i < k; i++) s += S2[i] * Tt[k - 1 - i];
        if ((k - 1) % 3 == 0) s += 2 * Tt[(k - 1) / 3];
        Tt[k] = div3(s);
        for (i = 0; i <= k; i++) s2 += Tt[i] * Tt[k - i];
        S2[k] = s2;
        for (i = 0; 2 * i <= k - 1; i++)
            sn += Nn[k - 1 - 2 * i] * Nn[i];
        Nn[k] = sn;
    }
    cnt = m + 1;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    ensure(n - 1);
    return Tt[n - 1] - Nn[n - 1];
}
