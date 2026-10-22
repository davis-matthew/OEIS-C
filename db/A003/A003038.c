/*
 * A003038  Dimensions of split simple Lie algebras over any field of
 *          characteristic zero.
 * https://oeis.org/A003038
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = n-th smallest distinct number among l(l+2) (l>=1),
 *            l(2l+1) (l>=2), l(2l-1) (l>=4) and the exceptional dimensions
 *            14, 52, 78, 133, 248; computed by merging the increasing lists
 *            in order, removing duplicates
 * Range:     u8 n <= 36, u16 n <= 612, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const uint64_t a003038_exc[5] = {14, 52, 78, 133, 248};

seq_t a(idx_t n)
{
    uint64_t la = 1, lb = 2, ld = 4;
    int e = 0;
    int kind = 0;
    uint64_t kl = 0;
    idx_t cnt = 0;
    while (cnt < n) {
        uint64_t va = la * (la + 2);
        uint64_t vb = lb * (2 * lb + 1);
        uint64_t vd = ld * (2 * ld - 1);
        uint64_t ve = e < 5 ? a003038_exc[e] : UINT64_MAX;
        uint64_t m = va;
        if (vb < m) m = vb;
        if (vd < m) m = vd;
        if (ve < m) m = ve;
        if (va == m) { kind = 0; kl = la; }
        else if (vb == m) { kind = 1; kl = lb; }
        else if (vd == m) { kind = 2; kl = ld; }
        else { kind = 3; kl = m; }
        if (va == m) la++;
        if (vb == m) lb++;
        if (vd == m) ld++;
        if (ve == m) e++;
        cnt++;
    }
    {
        seq_t l = (seq_t)kl;
        if (kind == 0) return l * (l + 2);
        if (kind == 1) return l * (2 * l + 1);
        if (kind == 2) return l * (2 * l - 1);
        return l;
    }
}
