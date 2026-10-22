/*
 * A005961  Number of acyclic disubstituted alkanes with n carbon atoms and
 *          identical substituents.
 * https://oeis.org/A005961
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [x^n] (1/(1-x*B(x)^2) + 1/(1-x*B(x^2)))/2 for n >= 1,
 *            where B(x) = 1 + x*(B(x)^3 + 2*B(x^3))/3 is the g.f. of A000625
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 20, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/grow_zero.h"

static seq_t *B = 0, *S2 = 0, *S3 = 0, *G1 = 0, *G2 = 0;
static idx_t cap = 0, bdone = 0, gdone = 0;

static void ensure_cap(idx_t need) {
    if (need <= cap) return;
    idx_t nc = cap ? cap : 16;
    while (nc < need) nc *= 2;
    B = grow_zero(B, cap, nc);
    S2 = grow_zero(S2, cap, nc);
    S3 = grow_zero(S3, cap, nc);
    G1 = grow_zero(G1, cap, nc);
    G2 = grow_zero(G2, cap, nc);
    cap = nc;
}

static void compute_b(idx_t upto) {
    while (bdone <= upto) {
        idx_t j = bdone;
        if (j == 0) {
            B[0] = 1;
        } else {
            seq_t t = S3[j - 1];
            if ((j - 1) % 3 == 0) t += 2 * B[(j - 1) / 3];
            B[j] = t / 3;
        }
        seq_t s = 0;
        for (idx_t i = 0; i <= j; i++) s += B[i] * B[j - i];
        S2[j] = s;
        seq_t s3 = 0;
        for (idx_t i = 0; i <= j; i++) s3 += B[i] * S2[j - i];
        S3[j] = s3;
        bdone++;
    }
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    ensure_cap(n + 1);
    if (gdone == 0) {
        G1[0] = 1;
        G2[0] = 1;
        gdone = 1;
    }
    compute_b(n - 1);
    while (gdone <= n) {
        idx_t m = gdone;
        seq_t s1 = 0, s2 = 0;
        for (idx_t k = 1; k <= m; k++) {
            s1 += S2[k - 1] * G1[m - k];
            if ((k - 1) % 2 == 0) s2 += B[(k - 1) / 2] * G2[m - k];
        }
        G1[m] = s1;
        G2[m] = s2;
        gdone++;
    }
    return G2[n] + (G1[n] - G2[n]) / 2;
}
