/*
 * A005168  n-th derivative of x^x at 1, divided by n.
 * https://oeis.org/A005168
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, nice
 * Formula:   a(n) = A005727(n)/n, where D(n) = A005727(n) = Sum_{k=0..n-1}
 *            binomial(n-1,k)*g(k)*D(n-1-k), D(0) = 1, g(0) = 1, g(k) =
 *            (-1)^(k-1)*(k-1)! for k >= 1 (since (x^x)' = x^x*(1+log(x)))
 * Range:     i8 n <= 8, i16 n <= 10, i32 n <= 15, i64 n <= 23, i128 n <= 36,
 *            big any n
 * Helpers:   from_i128, grow_zero, seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/grow_zero.h"
#include "../utils/from_i128.h"
#include "../utils/seq_is_fixed_width.h"

#define FW_MAX 128

static seq_t *Dc = NULL;
static idx_t Dn = 0;

static seq_t a_fixed(idx_t n) {
    /* exact arithmetic modulo 2^128, then exact division by n */
    unsigned __int128 D[FW_MAX + 2], row[FW_MAX + 2], f[FW_MAX + 2];
    if (n > FW_MAX) return 0;
    f[1] = 1;
    for (idx_t k = 2; k <= n; k++) f[k] = f[k - 1] * (unsigned __int128)(k - 1);
    D[0] = 1;
    row[0] = 1;
    for (idx_t m = 1; m <= n; m++) {
        unsigned __int128 s = 0;
        for (idx_t k = 0; k < m; k++) {
            unsigned __int128 t = row[k] * D[m - 1 - k];
            if (k == 0) s += t;
            else if (k % 2 == 1) s += t * f[k];
            else s -= t * f[k];
        }
        D[m] = s;
        row[m] = 0;
        for (idx_t k = m; k >= 1; k--) row[k] += row[k - 1];
    }
    unsigned __int128 u = D[n];
    idx_t odd = n;
    int sh = 0;
    while (odd % 2 == 0) { odd /= 2; sh++; }
    unsigned __int128 mo = (unsigned __int128)odd, inv = mo;
    for (int i = 0; i < 7; i++) inv *= (unsigned __int128)2 - mo * inv;
    unsigned __int128 v = (u >> sh) * inv;
    if (sh > 0) {
        int bits = 128 - sh;
        unsigned __int128 full = (unsigned __int128)1 << bits;
        v &= full - 1;
        if ((v >> (bits - 1)) & 1) v -= full;
    }
    return from_i128((__int128)v);
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    if (seq_is_fixed_width()) return a_fixed(n);
    if (Dn < n + 1) {
        Dc = grow_zero(Dc, Dn, n + 1);
        if (Dn == 0) { Dc[0] = 1; Dn = 1; }
        for (idx_t m = Dn; m <= n; m++) {
            seq_t s = Dc[m - 1];
            seq_t c = (seq_t)(m - 1);  /* binomial(m-1,k)*(k-1)! for k = 1 */
            for (idx_t k = 1; k < m; k++) {
                if (k % 2 == 1) s += c * Dc[m - 1 - k];
                else s -= c * Dc[m - 1 - k];
                if (k + 1 < m) c = c * (seq_t)k * (seq_t)(m - 1 - k) / (seq_t)(k + 1);
            }
            Dc[m] = s;
        }
        Dn = n + 1;
    }
    return Dc[n] / (seq_t)n;
}
