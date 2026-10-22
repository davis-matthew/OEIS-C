/*
 * A003718  E.g.f. tan(tan(x)), zeros omitted.
 * https://oeis.org/A003718
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k odd} D(2n+1,k) * D(k,1), where D(m,k) = m! [x^m]
 *            tan(x)^k / k! satisfies D(m+1,k) = D(m,k-1) + k*(k+1)*D(m,k+1),
 *            D(0,0) = 1, D(0,k) = 0 for k > 0, and D(k,1) are the tangent
 *            numbers (E.g.f. composition tan(tan(x)))
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 9, u128 n <= 16,
 *            big any n
 * Helpers:   alloc_zero, grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"

static seq_t *DA = NULL, *DB = NULL, *TT = NULL, *VV = NULL;
static idx_t cap = 0, row = 0, nv = 0;

seq_t a(idx_t n) {
    idx_t need = 2 * n + 4;
    if (cap == 0) {
        cap = need < 16 ? 16 : need;
        DA = alloc_zero(cap);
        DB = alloc_zero(cap);
        TT = alloc_zero(cap);
        VV = alloc_zero(cap);
        DA[0] = 1;
        row = 0;
        nv = 0;
    } else if (need > cap) {
        idx_t nc = cap * 2;
        if (nc < need) nc = need;
        DA = grow_zero(DA, cap, nc);
        DB = grow_zero(DB, cap, nc);
        TT = grow_zero(TT, cap, nc);
        VV = grow_zero(VV, cap, nc);
        cap = nc;
    }
    while (nv <= n) {
        idx_t k;
        seq_t *tmp;
        seq_t s;
        for (k = 0; k <= row + 1; k++) {
            seq_t v = (seq_t)k * (seq_t)(k + 1) * DA[k + 1];
            if (k > 0) v += DA[k - 1];
            DB[k] = v;
        }
        tmp = DA; DA = DB; DB = tmp;
        row++;
        TT[row] = DA[1];
        if (row & 1) {
            s = 0;
            for (k = 1; k <= row; k += 2)
                s += DA[k] * TT[k];
            VV[(row - 1) / 2] = s;
            nv++;
        }
    }
    return VV[n];
}
