/*
 * A000672  Number of 3-valent trees (= boron trees or binary trees) with n
 *          nodes.
 * https://oeis.org/A000672
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, changed
 * Formula:   a(n) = B(n) - B(n+1) + [n even]*B(n/2) + S3(n-1), where B(x) =
 *            Sum B(k) x^k = 1 + x*(B(x)^2 + B(x^2))/2 (B(k) = A001190(k+1)),
 *            i.e. B(0) = 1, B(k) = Sum_{i<j, i+j=k-1} B(i)*B(j) + [k-1
 *            even]*C(B((k-1)/2)+1, 2), and S3(m) = [x^m] (B(x)^3 +
 *            3*B(x)*B(x^2) + 2*B(x^3))/6 = ( Sum_{i=0..m} B(i)*B(m+1-i)...
 * Range:     u8 n <= 12, u16 n <= 20, u32 n <= 33, u64 n <= 59, u128 n <= 110,
 *            big any n
 * Helpers:   tri_plus
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/tri_plus.h"

static seq_t *Bt = NULL;
static idx_t Blen = 0;
static idx_t Bcap = 0;

static int fixed_flag = -1;
static seq_t inv3;

static int seq_fixed(void) {
    if (fixed_flag < 0) {
        seq_t x = 1;
        int i;
        for (i = 0; i < 200; i++) x = (seq_t)(x + x);
        fixed_flag = (x == 0) ? 1 : 0;
        if (fixed_flag) {
            seq_t v = 3;
            for (i = 0; i < 7; i++) {
                seq_t t = (seq_t)(2 - (seq_t)(3 * v));
                v = (seq_t)(v * t);
            }
            inv3 = v;
        }
    }
    return fixed_flag;
}

static void ensure_B(idx_t N) {
    idx_t k;
    if (N < Blen) return;
    if (N + 1 > Bcap) {
        idx_t nc = (N + 1) * 2;
        seq_t *nb = (seq_t *)malloc(nc * sizeof(seq_t));
        for (k = 0; k < Blen; k++) nb[k] = Bt[k];
        for (k = Blen; k < nc; k++) nb[k] = 0;
        free(Bt);
        Bt = nb;
        Bcap = nc;
    }
    for (k = Blen; k <= N; k++) {
        if (k == 0) {
            Bt[0] = 1;
            continue;
        }
        idx_t m = k - 1;
        seq_t s = 0;
        idx_t i;
        for (i = 0; 2 * i < m; i++) s += Bt[i] * Bt[m - i];
        if (m % 2 == 0) s += tri_plus(Bt[m / 2]);
        Bt[k] = s;
    }
    Blen = N + 1;
}

seq_t a(idx_t n) {
    idx_t i, m;
    seq_t tot, s3, res;
    seq_fixed();
    ensure_B(n + 1);
    if (n == 0) return 1;
    m = n - 1;
    tot = 0;
    for (i = 0; i <= m; i++) tot += Bt[i] * Bt[m + 1 - i];
    for (i = 0; 2 * i <= m; i++) tot += Bt[i] * Bt[m - 2 * i];
    if (m % 3 == 0) tot += Bt[m / 3];
    if (fixed_flag) s3 = (seq_t)(tot * inv3);
    else s3 = tot / 3;
    res = Bt[n] + s3;
    if (n % 2 == 0) res += Bt[n / 2];
    res -= Bt[n + 1];
    return res;
}
