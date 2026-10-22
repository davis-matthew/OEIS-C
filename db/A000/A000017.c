/*
 * A000017  Erroneous version of A032522.
 * https://oeis.org/A000017
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = A032522(n) (number of n-queens solutions invariant under
 *            a half-turn), found by a backtracking search that places queens
 *            in symmetric pairs; for the three terms OEIS printed wrongly, a
 *            fixed erratum is added: a(16) = A032522(16) - 128, a(17) =
 *            A032522(17) - 500, a(19) = A032522(19) + 6000....
 * Range:     i8 n <= 12, i16 n <= 18, i32 n <= 19, i64 n <= 19, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static int Q_N;
static char *Q_col, *Q_d1, *Q_d2;
static seq_t Q_cnt;

static void q_rec(int i)
{
    int c, j, c2, n = Q_N;
    if (i >= n / 2) {
        Q_cnt++;
        return;
    }
    j = n - 1 - i;
    for (c = 0; c < n; c++) {
        if (Q_col[c] || Q_d1[i + c] || Q_d2[i - c + n - 1])
            continue;
        Q_col[c] = 1; Q_d1[i + c] = 1; Q_d2[i - c + n - 1] = 1;
        c2 = n - 1 - c;
        if (!Q_col[c2] && !Q_d1[j + c2] && !Q_d2[j - c2 + n - 1]) {
            Q_col[c2] = 1; Q_d1[j + c2] = 1; Q_d2[j - c2 + n - 1] = 1;
            q_rec(i + 1);
            Q_col[c2] = 0; Q_d1[j + c2] = 0; Q_d2[j - c2 + n - 1] = 0;
        }
        Q_col[c] = 0; Q_d1[i + c] = 0; Q_d2[i - c + n - 1] = 0;
    }
}

/* Erratum of the dead entry relative to the true A032522: (index, offset). */
static const int Q_err_idx[3] = { 16, 17, 19 };
static const int Q_err_off[3] = { -128, -500, 6000 };

seq_t a(idx_t n)
{
    int N = (int)n, k;
    Q_N = N;
    Q_col = (char *)calloc((size_t)N + 1, 1);
    Q_d1 = (char *)calloc((size_t)(2 * N) + 1, 1);
    Q_d2 = (char *)calloc((size_t)(2 * N) + 1, 1);
    Q_cnt = 0;
    if (N % 2 == 1) {
        int m = N / 2;
        Q_col[m] = 1;
        Q_d1[m + m] = 1;
        Q_d2[N - 1] = 1;
    }
    q_rec(0);
    free(Q_col);
    free(Q_d1);
    free(Q_d2);
    for (k = 0; k < 3; k++)
        if (N == Q_err_idx[k])
            Q_cnt += Q_err_off[k];
    return Q_cnt;
}
