/*
 * A000185  a(n) = A058087(n,n-5).
 * https://oeis.org/A000185
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=5..n} (-1)^(j-5) * C(j,5) * (2n/(2n-j)) *
 *            C(2n-j,j) * (n-j)! (number of permutations with exactly 5 menage
 *            hits; 2n/(2n-j)*C(2n-j,j) = C(2n-j,j)+C(2n-j-1,j-1))
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 14, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   a185_c5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/a185_c5.h"

#define A185_PMAX 141

static seq_t a185_P[A185_PMAX][A185_PMAX];
static int a185_init = 0;

static void a185_build(void)
{
    int i, k;
    for (i = 0; i < A185_PMAX; i++) {
        for (k = 0; k < A185_PMAX; k++) a185_P[i][k] = 0;
        a185_P[i][0] = 1;
        for (k = 1; k <= i; k++)
            a185_P[i][k] = a185_P[i - 1][k - 1] + (k <= i - 1 ? a185_P[i - 1][k] : (seq_t)0);
    }
    a185_init = 1;
}

seq_t a(idx_t n)
{
    seq_t pos = 0, neg = 0, f = 1;
    idx_t j;
    if (n < 5) return 0;
    if (2 * n < A185_PMAX) {
        /* division-free path (exact modulo 2^w for fixed-width types) */
        if (!a185_init) a185_build();
        for (j = n; ; j--) {
            seq_t m = a185_P[2 * n - j][j] + a185_P[2 * n - j - 1][j - 1];
            seq_t term = a185_P[j][5] * m * f;
            if ((j - 5) & 1) neg += term; else pos += term;
            if (j == 5) break;
            f *= (seq_t)(n - j + 1);
        }
        return pos - neg;
    } else {
        seq_t B = 1; /* C(2n-j, j) */
        for (j = n; ; j--) {
            seq_t term = (seq_t)(2 * n) * B / (seq_t)(2 * n - j);
            term = term * a185_c5(j) * f;
            if ((j - 5) & 1) neg += term; else pos += term;
            if (j == 5) break;
            f *= (seq_t)(n - j + 1);
            B = B * (seq_t)(2 * n - j + 1) * (seq_t)j;
            B = B / ((seq_t)(2 * n - 2 * j + 2) * (seq_t)(2 * n - 2 * j + 1));
        }
        return pos - neg;
    }
}
