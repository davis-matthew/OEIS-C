/*
 * A005956  Number of acyclic tertiary alcohols with n carbon atoms.
 * https://oeis.org/A005956
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [x^(n-1)] ((B(x)-1)^3 + 2*(B(x^3)-1))/3, where B(x) =
 *            g.f. of A000625: B(x) = 1 + x*(B(x)^3 + 2*B(x^3))/3, i.e. b(0) =
 *            1, b(m+1) = ([x^m] B^3 + 2*[3|m]*b(m/3))/3. (B-1)^3 = B^3 - 3B^2
 *            + 3B - 1. The division by 3 is exact. In fixed width it is done
 *            by multiplying by 3^(-1) mod 2^w, which st...
 * Range:     u8 n <= 10, u16 n <= 15, u32 n <= 25, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   grow_zero, seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/grow_zero.h"
#include "../utils/seq_is_fixed_width.h"

static seq_t *bb = 0, *s2 = 0, *s3 = 0;
static idx_t cap = 0, have = 0;
static bool inv_ready = false;
static seq_t inv3;

static seq_t div3(seq_t x) {
    if (seq_is_fixed_width()) {
        if (!inv_ready) {
            seq_t v = 3;
            int k;
            for (k = 0; k < 7; k++) v = v * (2 - 3 * v);
            inv3 = v;
            inv_ready = true;
        }
        return x * inv3;
    }
    return x / 3;
}

static void ensure(idx_t m) {
    idx_t i, j;
    if (m + 2 > cap) {
        idx_t nc = (m + 2) * 2;
        bb = grow_zero(bb, cap, nc);
        s2 = grow_zero(s2, cap, nc);
        s3 = grow_zero(s3, cap, nc);
        cap = nc;
    }
    if (have == 0) bb[0] = 1;
    for (i = have; i <= m; i++) {
        seq_t t2 = 0, t3 = 0, extra = 0;
        for (j = 0; j <= i; j++) t2 += bb[j] * bb[i - j];
        s2[i] = t2;
        for (j = 0; j <= i; j++) t3 += bb[j] * s2[i - j];
        s3[i] = t3;
        if (i % 3 == 0) extra = 2 * bb[i / 3];
        bb[i + 1] = div3(t3 + extra);
    }
    if (m + 1 > have) have = m + 1;
}

seq_t a(idx_t n) {
    idx_t m;
    seq_t c3, fix = 0;
    if (n < 4) return 0;
    m = n - 1;
    ensure(m);
    c3 = s3[m] + 3 * bb[m] - 3 * s2[m];
    if (m % 3 == 0) fix = bb[m / 3];
    return div3(c3 + 2 * fix);
}
