/*
 * A002811  Erroneous version of A002439.
 * https://oeis.org/A002811
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(1) = 1, a(2) = 5 (the erroneous terms); for n >= 3, a(n) =
 *            T(2n-3), where T(1) = 1 and, for odd j, T(j) = (-4)^((j-1)/2) -
 *            Sum_{k=1..(j-1)/2} binomial(j,2k)*(-9)^k*T(j-2k)
 * Range:     i8 n <= 3, i16 n <= 4, i32 n <= 6, i64 n <= 8, i128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *F = NULL;
static idx_t flen = 0;

seq_t a(idx_t n) {
    if (n == 1) return 1;
    if (n == 2) return 5;
    idx_t N = 2 * n - 3;
    if (flen < N + 1) {
        seq_t *nf = (seq_t *)malloc((size_t)(N + 1) * sizeof(seq_t));
        seq_t *row = (seq_t *)malloc((size_t)(N + 1) * sizeof(seq_t));
        idx_t i, j, k;
        seq_t s = 1;
        for (i = 0; i < flen; i++) nf[i] = F[i];
        for (i = flen; i <= N; i++) nf[i] = 0;
        if (F) free(F);
        F = nf;
        row[0] = 1;
        for (j = 1; j <= N; j++) {
            row[j] = 1;
            for (i = j - 1; i >= 1; i--) row[i] += row[i - 1];
            if (j % 2 == 1) {
                if (j > 1) s *= -4;
                if (j >= flen) {
                    seq_t sum = s;
                    seq_t p = 1;
                    for (k = 1; 2 * k < j; k++) {
                        p *= -9;
                        sum -= row[2 * k] * p * F[j - 2 * k];
                    }
                    F[j] = sum;
                }
            }
        }
        free(row);
        flen = N + 1;
    }
    return F[N];
}
