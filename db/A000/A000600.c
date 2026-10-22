/*
 * A000600  Number of tertiary alcohols (alkanols or alkyl alcohols C_n
 *          H_{2n+1} OH) with n carbon atoms.
 * https://oeis.org/A000600
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   A(x) = x*Z(S3; C(x)) with C(x) = B(x) - 1, B(x) = g.f. of
 *            A000598, B(x) = 1 + x*(1 + C + Z(S2;C) + Z(S3;C)). Uses M2 =
 *            Z(S2;C): M2[w] = Sum_{i<w-i} c_i*c_{w-i} + [w
 *            even]*c_{w/2}*(c_{w/2}+1)/2, and M3 = Z(S3;C) = (C*M2 +
 *            C(x)*C(x^2) + C(x^3))/3. c_w = A000598(w) = [w=1] + c_{w-1} +
 *            M2[w-1] + M3[w...
 * Range:     u8 n <= 11, u16 n <= 16, u32 n <= 28, u64 n <= 50, u128 n <= 94,
 *            big any n
 * Helpers:   alloc_zero, tri_half
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/alloc_zero.h"
#include "../utils/tri_half.h"

static seq_t *Cc = 0, *Q2 = 0, *Q3 = 0;
static idx_t have = 0, cap = 0;
static int big_mode = -1;
static seq_t inv3;

static void setup(void) {
    seq_t x = 1;
    int i;
    for (i = 0; i < 200; i++) x <<= 1;
    big_mode = (x != 0) ? 1 : 0;
    if (!big_mode) {
        seq_t v = 3;
        for (i = 0; i < 8; i++) {
            seq_t t = v * 3;
            t = (seq_t)2 - t;
            v = v * t;
        }
        inv3 = v;
    }
}

static void ensure(idx_t need) {
    idx_t w, i, j;
    if (need <= have) return;
    if (need > cap) {
        idx_t nc = cap * 2;
        if (nc < need) nc = need;
        if (nc < 16) nc = 16;
        seq_t *nC = alloc_zero(nc), *n2 = alloc_zero(nc), *n3 = alloc_zero(nc);
        for (i = 0; i < have; i++) {
            nC[i] = Cc[i]; n2[i] = Q2[i]; n3[i] = Q3[i];
        }
        Cc = nC; Q2 = n2; Q3 = n3;
        cap = nc;
    }
    if (have == 0) {
        Cc[0] = 0; Q2[0] = 0; Q3[0] = 0;
        have = 1;
    }
    for (w = have; w < need; w++) {
        seq_t c = Cc[w - 1];
        c += Q2[w - 1];
        c += Q3[w - 1];
        if (w == 1) c += 1;
        Cc[w] = c;

        seq_t s2 = 0;
        for (i = 1; 2 * i < w; i++) s2 += Cc[i] * Cc[w - i];
        if (w % 2 == 0) s2 += tri_half(Cc[w / 2]);
        Q2[w] = s2;

        seq_t s3 = 0;
        for (i = 1; i + 2 <= w; i++) s3 += Cc[i] * Q2[w - i];
        for (j = 1; 2 * j < w; j++) s3 += Cc[w - 2 * j] * Cc[j];
        if (w % 3 == 0) s3 += Cc[w / 3];
        if (big_mode) Q3[w] = s3 / 3;
        else Q3[w] = s3 * inv3;
    }
    have = need;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (big_mode < 0) setup();
    ensure(n);
    return Q3[n - 1];
}
