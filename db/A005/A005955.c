/*
 * A005955  Number of acyclic secondary alcohols with n carbon atoms.
 * https://oeis.org/A005955
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [x^(n-1)] (B(x)-1)^2, where B(x) = 1 + x*(B(x)^3 +
 *            2*B(x^3))/3 is the g.f. of A000625
 * Range:     u8 none, u16 none, u32 n <= 24, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   grow_zero, seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/grow_zero.h"
#include "../utils/seq_is_fixed_width.h"

static seq_t *Bc = 0, *B2c = 0, *B3c = 0;
static idx_t cap = 0, done = 0;

static seq_t div3(seq_t t) {
    if (seq_is_fixed_width()) {
        /* exact modular division by 3 (word sizes are even, so 3 | 2^w - 1) */
        seq_t k = ((seq_t)0 - 1) / 3;
        seq_t inv = k * 2 + 1;
        return t * inv;
    }
    return t / 3;
}

static void ensure(idx_t m) {
    if (m < done) return;
    if (m + 1 > cap) {
        idx_t nc = cap ? cap : 16;
        while (nc < m + 1) nc *= 2;
        Bc = grow_zero(Bc, cap, nc);
        B2c = grow_zero(B2c, cap, nc);
        B3c = grow_zero(B3c, cap, nc);
        cap = nc;
    }
    for (idx_t i = done; i <= m; i++) {
        if (i == 0) {
            Bc[0] = 1; B2c[0] = 1; B3c[0] = 1;
        } else {
            seq_t t = B3c[i - 1];
            if ((i - 1) % 3 == 0) t += Bc[(i - 1) / 3] * 2;
            Bc[i] = div3(t);
            seq_t s = 0;
            for (idx_t j = 0; j < (i + 1) / 2; j++) s += Bc[j] * Bc[i - j];
            s = s * 2;
            if (i % 2 == 0) s += Bc[i / 2] * Bc[i / 2];
            B2c[i] = s;
            seq_t s3 = 0;
            for (idx_t j = 0; j <= i; j++) s3 += B2c[j] * Bc[i - j];
            B3c[i] = s3;
        }
    }
    done = m + 1;
}

seq_t a(idx_t n) {
    if (n < 3) return 0;
    ensure(n - 2);
    seq_t r = 0;
    for (idx_t k = 1; k <= n - 2; k++) r += Bc[k] * Bc[n - 1 - k];
    return r;
}
