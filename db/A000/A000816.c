/*
 * A000816  E.g.f.: Sum_{n >= 0} a(n) * x^(2*n) / (2*n)! = sin(x)^2 /
 *          cos(2*x).
 * https://oeis.org/A000816
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(2*n-1) * A000364(n) for n > 0, a(0) = 0; A000364(n) =
 *            A000111(2n) (secant numbers), computed with the Seidel-Entringer
 *            boustrophedon triangle: E(0,0) = 1, E(k,0) = 0, E(k,j) =
 *            E(k,j-1) + E(k-1,k-j), A000111(k) = E(k,k)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   grow_arr
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/grow_arr.h"

static seq_t *s_row = 0, *s_tmp = 0, *s_zz = 0;
static idx_t s_cap = 0, s_cur = 0;

static void ensure(idx_t K) {
    if (s_cap == 0) {
        idx_t c = 16;
        while (c < K + 2) c *= 2;
        s_row = grow_arr(0, 0, c);
        s_tmp = grow_arr(0, 0, c);
        s_zz = grow_arr(0, 0, c);
        s_cap = c;
        s_row[0] = 1;
        s_zz[0] = 1;
        s_cur = 1;
    }
    if (K + 2 > s_cap) {
        idx_t c = s_cap;
        while (c < K + 2) c *= 2;
        s_row = grow_arr(s_row, s_cap, c);
        s_tmp = grow_arr(s_tmp, s_cap, c);
        s_zz = grow_arr(s_zz, s_cap, c);
        s_cap = c;
    }
    while (s_cur <= K) {
        idx_t k = s_cur;
        idx_t j;
        seq_t *t;
        s_tmp[0] = 0;
        for (j = 1; j <= k; j++)
            s_tmp[j] = s_tmp[j - 1] + s_row[k - j];
        s_zz[k] = s_tmp[k];
        t = s_row; s_row = s_tmp; s_tmp = t;
        s_cur++;
    }
}

seq_t a(idx_t n) {
    seq_t r;
    idx_t i;
    if (n == 0) return 0;
    ensure(2 * n);
    r = s_zz[2 * n];
    for (i = 0; i < 2 * n - 1; i++) {
        if (r == 0) break;
        r *= 2;
    }
    return r;
}
