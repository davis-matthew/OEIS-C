/*
 * A004420  Expansion of (Sum_{n=-inf..inf} x^(n^2))^(-19).
 * https://oeis.org/A004420
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = [x^n] theta(x)^(-19), theta(x) = 1 + 2*Sum_{k>=1}
 *            x^(k^2). Computed by dividing by theta 19 times: if g = h/theta
 *            then g(m) = h(m) - 2*Sum_{k>=1, k^2<=m} g(m-k^2), starting from
 *            h = 1
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 8, i64 n <= 24, i128 n <= 74,
 *            big any n
 * Helpers:   grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/grow_zero.h"

static seq_t *L[20];
static idx_t have = 0;

seq_t a(idx_t n) {
    if (n >= have) {
        idx_t newlen = n + 1;
        for (int k = 1; k <= 19; k++)
            L[k] = grow_zero(L[k], have, newlen);
        for (idx_t i = have; i < newlen; i++) {
            for (int k = 1; k <= 19; k++) {
                seq_t s = 0;
                for (idx_t j = 1; j * j <= i; j++)
                    s += L[k][i - j * j];
                seq_t h;
                if (k == 1)
                    h = (i == 0) ? 1 : 0;
                else
                    h = L[k - 1][i];
                L[k][i] = h - (s + s);
            }
        }
        have = newlen;
    }
    return L[19][n];
}
