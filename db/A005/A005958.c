/*
 * A005958  Number of esters with n carbon atoms up to stereo-isomerism.
 * https://oeis.org/A005958
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   G.f. x*B(x)*(B(x)-1), where B(x) = 1 + x*(B(x)^3 + 2*B(x^3))/3
 *            is the g.f. of A000625; so a(n) = [x^(n-1)] B(x)^2 - b(n-1),
 *            with b(0) = 1
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 23, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/grow_zero.h"

static seq_t *Bc = NULL;   /* coefficients of B(x)   */
static seq_t *S2 = NULL;   /* coefficients of B(x)^2 */
static seq_t *S3 = NULL;   /* coefficients of B(x)^3 */
static idx_t done = 0;     /* indices 0..done-1 computed */
static idx_t cap = 0;

static void ensure(idx_t N) {
    idx_t m, i;
    if (N + 1 > cap) {
        idx_t nc = cap * 2;
        if (nc < N + 1) nc = N + 1;
        if (nc < 32) nc = 32;
        Bc = grow_zero(Bc, cap, nc);
        S2 = grow_zero(S2, cap, nc);
        S3 = grow_zero(S3, cap, nc);
        cap = nc;
    }
    if (done == 0) {
        Bc[0] = 1;
        S2[0] = 1;
        S3[0] = 1;
        done = 1;
    }
    for (m = done; m <= N; m++) {
        seq_t t = S3[m - 1];
        if ((m - 1) % 3 == 0) t += 2 * Bc[(m - 1) / 3];
        Bc[m] = t / 3;

        seq_t s = 0;
        for (i = 0; 2 * i < m; i++) s += Bc[i] * Bc[m - i];
        s = s * 2;
        if (m % 2 == 0) s += Bc[m / 2] * Bc[m / 2];
        S2[m] = s;

        seq_t u = 0;
        for (i = 0; i <= m; i++) u += Bc[i] * S2[m - i];
        S3[m] = u;
    }
    if (N + 1 > done) done = N + 1;
}

seq_t a(idx_t n) {
    if (n < 2) return 0;
    ensure(n - 1);
    return S2[n - 1] - Bc[n - 1];
}
