/*
 * A004213  Shifts one place left under 4th-order binomial transform.
 * https://oeis.org/A004213
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, eigen
 * Formula:   a(n+1) = Sum_{k=0..n} binomial(n,k)*4^(n-k)*a(k), a(0) = 1 (from
 *            A'(x)/A(x) = e^(4x)), evaluated with a Bell-like triangle: row n
 *            starts with a(n), T(n,j) = T(n,j-1) + 4*T(n-1,j-1), and a(n+1)
 *            is the last entry of row n
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 31,
 *            big any n
 * Helpers:   grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/grow_zero.h"

static seq_t *av = NULL;    /* av[i] = a(i) for i < alen */
static idx_t alen = 0, acap = 0;
static seq_t *row = NULL;   /* last computed row, index rown, length rown+1 */
static idx_t rown = 0, rcap = 0;

seq_t a(idx_t n) {
    if (alen == 0) {
        acap = 16;
        av = grow_zero(NULL, 0, acap);
        rcap = 16;
        row = grow_zero(NULL, 0, rcap);
        av[0] = 1;
        row[0] = 1;          /* row 0 = [a(0)] */
        rown = 0;
        av[1] = 1;           /* a(1) = last entry of row 0 */
        alen = 2;
    }
    while (alen <= n) {
        idx_t m = rown + 1;  /* build row m from row m-1 */
        if (m + 1 > rcap) {
            idx_t nc = rcap * 2;
            while (nc < m + 1) nc *= 2;
            row = grow_zero(row, rcap, nc);
            rcap = nc;
        }
        seq_t tmp = row[0];
        row[0] = row[m - 1];             /* a(m) */
        for (idx_t j = 1; j < m; j++) {
            seq_t t = row[j];
            row[j] = row[j - 1] + tmp * 4;
            tmp = t;
        }
        row[m] = row[m - 1] + tmp * 4;
        rown = m;
        if (alen + 1 > acap) {
            idx_t nc = acap * 2;
            av = grow_zero(av, acap, nc);
            acap = nc;
        }
        av[alen] = row[m];               /* a(m+1) */
        alen++;
    }
    return av[n];
}
