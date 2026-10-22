/*
 * A004048  Theta series of 14-dimensional extremal 3-modular lattice with det
 *          3^7, minimal norm 4, group 2 X G_2(3).
 * https://oeis.org/A004048
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   G.f.: theta_A2(q)^7 - 42*theta_A2(q)*eta(q)^6*eta(q^3)^6, where
 *            theta_A2(q) = 1 + 6*Sum_{n>=1} (d_{1,3}(n) - d_{2,3}(n)) q^n
 *            (series in powers of q with q = norm/2)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 21, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   from_i128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/from_i128.h"

static __int128 *F = NULL;
static idx_t cap = 0; /* F valid for indices 0..cap-1 */

static void build(idx_t N)
{
    idx_t L = N + 1, n, k, d, m;
    long long j;
    __int128 *th = (__int128 *)calloc(L, sizeof(__int128));
    __int128 *f = (__int128 *)calloc(L, sizeof(__int128));
    __int128 *g = (__int128 *)calloc(L, sizeof(__int128));
    __int128 *h = (__int128 *)calloc(L, sizeof(__int128));
    long long *pk = (long long *)malloc(sizeof(long long) * (2 * L + 4));
    int *ps = (int *)malloc(sizeof(int) * (2 * L + 4));
    int np = 0;

    /* theta series of A2 */
    th[0] = 1;
    for (d = 1; d <= N; d++) {
        int c = (d % 3 == 1) ? 1 : (d % 3 == 2) ? -1 : 0;
        if (!c) continue;
        for (m = d; m <= N; m += d) th[m] += 6 * c;
    }

    /* f = theta^7 by power recurrence */
    f[0] = 1;
    for (n = 1; n <= N; n++) {
        __int128 s = 0;
        for (k = 1; k <= n; k++)
            if (th[k] != 0)
                s += (__int128)(8 * (long long)k - (long long)n) * th[k] * f[n - k];
        f[n] = s / (__int128)n;
    }

    /* Euler function coefficients (pentagonal numbers) */
    for (j = 1;; j++) {
        long long g1 = j * (3 * j - 1) / 2, g2 = j * (3 * j + 1) / 2;
        int sg = (j & 1) ? -1 : 1;
        if (g1 > (long long)N) break;
        pk[np] = g1; ps[np] = sg; np++;
        if (g2 <= (long long)N) { pk[np] = g2; ps[np] = sg; np++; }
    }

    /* g = E(q)^6 by power recurrence */
    g[0] = 1;
    for (n = 1; n <= N; n++) {
        __int128 s = 0;
        int i;
        for (i = 0; i < np && pk[i] <= (long long)n; i++)
            s += (__int128)(7 * pk[i] - (long long)n) * ps[i] * g[n - pk[i]];
        f[0] = f[0]; /* no-op */
        g[n] = s / (__int128)n;
    }

    /* h = E(q)^6 * E(q^3)^6 */
    for (n = 0; n <= N; n++) {
        __int128 s = 0;
        for (k = 0; 3 * k <= n; k++) s += g[k] * g[n - 3 * k];
        h[n] = s;
    }

    /* F = f - 42 * theta * q * h */
    {
        __int128 *nf = (__int128 *)malloc(sizeof(__int128) * L);
        for (n = 0; n <= N; n++) {
            __int128 t = 0;
            for (k = 0; k < n; k++)
                if (th[k] != 0) t += th[k] * h[n - k - 1];
            nf[n] = f[n] - 42 * t;
        }
        free(F);
        F = nf;
        cap = L;
    }
    free(th); free(f); free(g); free(h); free(pk); free(ps);
}

seq_t a(idx_t n)
{
    if (n >= cap) {
        idx_t N = 64;
        while (N < n) N *= 2;
        if (N < n) N = n;
        build(N);
    }
    return from_i128(F[n]);
}
