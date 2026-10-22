/*
 * A003710  Expansion of e.g.f. cos(tan(x)) (even powers only).
 * https://oeis.org/A003710
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..n} (-1)^k * A(2n,2k), where A(N,j) = N! [x^N]
 *            tan(x)^j/j! satisfies A(N+1,j) = A(N,j-1) + j*(j+1)*A(N,j+1),
 *            A(0,0) = 1, A(0,j) = 0 for j > 0 (since cos(tan x) = Sum_k
 *            (-1)^k tan(x)^(2k)/(2k)!)
 * Range:     i8 n <= 3, i16 n <= 4, i32 n <= 7, i64 n <= 11, i128 n <= 19,
 *            big any n
 * Helpers:   alloc_zero, grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"

static seq_t *row = NULL;   /* row[j] = A(N,j) for j == N (mod 2) */
static idx_t rowcap = 0;
static idx_t curN = 0;
static seq_t *res = NULL;
static idx_t rescap = 0;
static idx_t done = 0;

seq_t a(idx_t n) {
    if (n < done) return res[n];

    idx_t need = 2 * n + 4;
    if (row == NULL) {
        rowcap = need < 16 ? 16 : need;
        row = alloc_zero(rowcap);
        row[0] = 1;
        curN = 0;
    } else if (rowcap < need) {
        idx_t nc = rowcap * 2;
        if (nc < need) nc = need;
        row = grow_zero(row, rowcap, nc);
        rowcap = nc;
    }
    if (res == NULL) {
        rescap = n + 1 < 8 ? 8 : n + 1;
        res = alloc_zero(rescap);
    } else if (rescap < n + 1) {
        idx_t nc = rescap * 2;
        if (nc < n + 1) nc = n + 1;
        res = grow_zero(res, rescap, nc);
        rescap = nc;
    }

    while (done <= n) {
        idx_t target = 2 * done;
        while (curN < target) {
            idx_t M = curN + 1;
            /* entries of parity M are recomputed from entries of parity curN */
            idx_t j = (M & 1) ? 1 : 0;
            if (j == 0) {
                row[0] = 0;
                j = 2;
            }
            for (; j <= M; j += 2) {
                seq_t t = row[j + 1];
                t *= (seq_t)j;
                t *= (seq_t)(j + 1);
                t += row[j - 1];
                row[j] = t;
            }
            curN = M;
        }
        seq_t s = 0;
        for (idx_t k = 0; k <= done; k++) {
            if (k & 1) s -= row[2 * k];
            else s += row[2 * k];
        }
        res[done] = s;
        done++;
    }
    return res[n];
}
