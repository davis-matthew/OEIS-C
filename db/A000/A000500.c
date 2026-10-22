/*
 * A000500  Number of permutations of an n-sequence discordant with three
 *          given permutations (see reference) in n-7 places.
 * https://oeis.org/A000500
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=7..n} (-1)^(k-7) * C(k,7) * (n-k)! * sigma_{n,k},
 *            i.e. the coefficient of y^7 in Sum_k sigma_{n,k} (n-k)! (y-1)^k,
 *            where sigma_{n,k} = [u^n t^k] (1 - 2t^2u^2 - 2t^2(1+t)u^3 +
 *            3t^4u^4) / ((1 - tu)(1 - (1+2t)u - tu^2 + t^3u^3)). The
 *            denominator expands to 1 - (1+3t)u + 2t^2u^2 + (t^2+...
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 14, u64 n <= 21, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *rows[5];
static long long cap = 0;
static long long last = -1;

static seq_t get_s(long long j, long long k) {
    if (j < 0 || k < 0 || k > j) return 0;
    return rows[j % 5][k];
}

seq_t a(idx_t n) {
    long long N = (long long)n;
    long long m, k, r, i;
    if (N < 7) return 0;

    if (N + 1 > cap) {
        long long nc = 2 * cap;
        if (nc < N + 1) nc = N + 1;
        nc += 8;
        for (r = 0; r < 5; r++) {
            if (rows[r]) free(rows[r]);
            rows[r] = (seq_t *)calloc((size_t)nc, sizeof(seq_t));
        }
        cap = nc;
        last = -1;
    } else if (N < last) {
        for (r = 0; r < 5; r++)
            for (i = 0; i < cap; i++) rows[r][i] = 0;
        last = -1;
    }

    for (m = last + 1; m <= N; m++) {
        seq_t *cur = rows[m % 5];
        for (k = 0; k <= m; k++) {
            seq_t v = get_s(m - 1, k);
            v += 3 * get_s(m - 1, k - 1);
            v -= 2 * get_s(m - 2, k - 2);
            v -= get_s(m - 3, k - 2);
            v -= get_s(m - 3, k - 3);
            v += get_s(m - 4, k - 4);
            if (m == 0 && k == 0) v += 1;
            if (m == 2 && k == 2) v -= 2;
            if (m == 3 && (k == 2 || k == 3)) v -= 2;
            if (m == 4 && k == 4) v += 3;
            cur[k] = v;
        }
    }
    last = N;

    seq_t *S = rows[N % 5];

    /* factorials 0! .. (N-7)! */
    seq_t *f = (seq_t *)calloc((size_t)(N - 6), sizeof(seq_t));
    f[0] = 1;
    for (i = 1; i <= N - 7; i++) f[i] = f[i - 1] * (seq_t)i;

    /* Pascal row for C(k, j), j = 0..7, advanced to k = 7 */
    seq_t bc[8];
    for (i = 0; i < 8; i++) bc[i] = 0;
    bc[0] = 1;
    for (k = 1; k <= 7; k++)
        for (i = 7; i >= 1; i--) bc[i] += bc[i - 1];

    seq_t result = 0;
    for (k = 7; k <= N; k++) {
        seq_t term = S[k] * f[N - k] * bc[7];
        if ((k - 7) & 1) result -= term;
        else result += term;
        for (i = 7; i >= 1; i--) bc[i] += bc[i - 1];
    }
    free(f);
    return result;
}
