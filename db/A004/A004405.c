/*
 * A004405  Expansion of 1 / (Sum_{n=-oo..oo} x^(n^2))^4.
 * https://oeis.org/A004405
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   1/(Sum_{n=-oo..oo} x^(n^2)) = Sum b(n) x^n with b(0) = 1, b(n) =
 *            -2*Sum_{k>=1, k^2<=n} b(n-k^2); c(n) = Sum_{i=0..n} b(i)*b(n-i)
 *            (1/theta^2); a(n) = Sum_{i=0..n} c(i)*c(n-i) (1/theta^4)
 * Range:     i8 n <= 2, i16 n <= 7, i32 n <= 23, i64 n <= 76, i128 n <= 259,
 *            big any n
 * Helpers:   alloc_zero, grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"

static seq_t *Bv = NULL, *Cv = NULL, *Av = NULL;
static idx_t cap = 0, len = 0;

static void ensure(idx_t n) {
    if (n + 1 > cap) {
        idx_t nc = cap * 2;
        if (nc < n + 1) nc = n + 1;
        if (nc < 64) nc = 64;
        if (cap == 0) {
            Bv = alloc_zero(nc);
            Cv = alloc_zero(nc);
            Av = alloc_zero(nc);
        } else {
            Bv = grow_zero(Bv, cap, nc);
            Cv = grow_zero(Cv, cap, nc);
            Av = grow_zero(Av, cap, nc);
        }
        cap = nc;
    }
    for (idx_t m = len; m <= n; m++) {
        if (m == 0) {
            Bv[0] = 1;
        } else {
            seq_t s = 0;
            for (idx_t k = 1; k * k <= m; k++) s += Bv[m - k * k];
            Bv[m] = -(s + s);
        }
        {
            seq_t s = 0;
            for (idx_t i = 0; 2 * i < m; i++) s += Bv[i] * Bv[m - i];
            s += s;
            if (m % 2 == 0) s += Bv[m / 2] * Bv[m / 2];
            Cv[m] = s;
        }
        {
            seq_t s = 0;
            for (idx_t i = 0; 2 * i < m; i++) s += Cv[i] * Cv[m - i];
            s += s;
            if (m % 2 == 0) s += Cv[m / 2] * Cv[m / 2];
            Av[m] = s;
        }
    }
    if (n + 1 > len) len = n + 1;
}

seq_t a(idx_t n) {
    if (n >= len) ensure(n);
    return Av[n];
}
