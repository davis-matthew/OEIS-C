/*
 * A005160  Number of alternating sign n X n matrices invariant under a
 *          quarter turn.
 * https://oeis.org/A005160
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, more
 * Formula:   a(4k) = A_HT(2k)*A(k)^2, a(4k+1) = A_HT(2k+1)*A(k)^2, a(4k-1) =
 *            A_HT(2k-1)*A(k)^2, a(4k+2) = 0 (Kuperberg / Razumov-Stroganov),
 *            where A(k) = Product_{j=0..k-1} (3j+1)!/(k+j)! (A005130) and
 *            A_HT(m) = Product_{i=1..m} R(i) (A005158) with R(2t) =
 *            (3t-1)!*(t-1)!/((2t-1)!)^2, R(2t+1) = (3t)!*t!/((2t)!...
 * Range:     u8 n <= 10, u16 n <= 14, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   a005160_ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/a005160_ipow.h"

seq_t a(idx_t n)
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

    /* A(k)^2 */
    for (j = 0; j < k; j++) {
        fe[3 * j + 1] += 2;
        fe[k + j] -= 2;
    }
    /* A_HT(m) */
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

    /* exponent of each integer x: sum of factorial exponents for y >= x */
    run = 0;
    for (x = L; x >= 2; x--) {
        run += fe[x];
        E[x] = run;
    }

    /* smallest prime factor sieve */
    for (x = 2; x <= L; x++) {
        if (spf[x] == 0) {
            for (y = x; y <= L; y += x)
                if (spf[y] == 0) spf[y] = x;
        }
    }

    /* push exponents of composites down to primes */
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
            res *= a005160_ipow((seq_t)(long long)x, E[x]);
    }

    free(fe);
    free(E);
    free(spf);
    return res;
}
