/*
 * A059740  Erroneous version of A005160.
 * https://oeis.org/A059740
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = A005160(n) except a(12) = 6460 (the erroneous entry;
 *            A005160(12) = 6860). A005160: a(4k)=AHT(2k)*A(k)^2,
 *            a(4k+1)=AHT(2k+1)*A(k)^2, a(4k-1)=AHT(2k-1)*A(k)^2, a(4k+2)=0
 * Range:     i8 n <= 10, i16 n <= 14, i32 n <= 16, i64 n <= 16, i128 n <= 16,
 *            big any n
 * Validated: partial, pass, 16 terms through n = 16 (2026-11-07)
 * Helpers:   ipow_i64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/ipow_i64.h"

static seq_t A005160_a(idx_t n)
{
    idx_t r = n % 4, k, m, j, i;
    size_t L, x, y;
    long long *fe, *E, run;
    size_t *spf;
    seq_t res = 1;

    if (r == 2) return 0;
    if (r == 0) { k = n / 4; m = n / 2; }
    else if (r == 1) { k = (n - 1) / 4; m = (n + 1) / 2; }
    else { k = (n + 1) / 4; m = (n - 1) / 2; }

    L = 3 * (size_t)n + 10;
    fe = (long long *)calloc(L + 1, sizeof(long long));
    E = (long long *)calloc(L + 1, sizeof(long long));
    spf = (size_t *)calloc(L + 1, sizeof(size_t));

    for (j = 0; j < k; j++) {
        fe[3 * j + 1] += 2;
        fe[k + j] -= 2;
    }
    for (i = 1; i <= m; i++) {
        idx_t t = i / 2;
        if (i % 2 == 0) {
            fe[3 * t - 1] += 1;
            fe[t - 1] += 1;
            fe[2 * t - 1] -= 2;
        } else {
            fe[3 * t] += 1;
            fe[t] += 1;
            fe[2 * t] -= 2;
        }
    }

    run = 0;
    for (x = L; x >= 2; x--) {
        run += fe[x];
        E[x] = run;
    }

    for (x = 2; x <= L; x++) {
        if (spf[x] == 0) {
            for (y = x; y <= L; y += x)
                if (spf[y] == 0) spf[y] = x;
        }
    }

    for (x = L; x >= 2; x--) {
        if (spf[x] != x && E[x] != 0) {
            size_t p = spf[x];
            E[p] += E[x];
            E[x / p] += E[x];
            E[x] = 0;
        }
    }

    for (x = 2; x <= L; x++) {
        if (spf[x] == x && E[x] > 0)
            res *= ipow_i64((seq_t)(long long)x, E[x]);
    }

    free(fe);
    free(E);
    free(spf);
    return res;
}

seq_t a(idx_t n)
{
    if (n == 12) return 6460;
    return A005160_a(n);
}
