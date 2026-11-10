/*
 * A076223  Minimum of the n-th row of A076221.
 * https://oeis.org/A076223
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min over 1<=k<=n of #{1<=x<=n : gcd(x,k)=1, x!=k};
 *            computed by a Moebius sieve: for k>1 the count is sum_{d|k}
 *            mu(d)*floor(n/d), for k=1 it is n-1
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Validated: partial, pass, 81 terms through n = 81 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    size_t N = (size_t)n;
    int *mu = (int *)malloc((N + 1) * sizeof(int));
    long long *f = (long long *)malloc((N + 1) * sizeof(long long));
    unsigned char *comp = (unsigned char *)malloc(N + 1);
    size_t i, j;
    for (i = 0; i <= N; i++) { mu[i] = 1; f[i] = 0; comp[i] = 0; }
    for (i = 2; i <= N; i++) {
        if (!comp[i]) {
            for (j = i; j <= N; j += i) {
                if (j > i) comp[j] = 1;
                mu[j] = -mu[j];
            }
            if (i <= N / i) {
                size_t sq = i * i;
                for (j = sq; j <= N; j += sq) mu[j] = 0;
            }
        }
    }
    for (i = 1; i <= N; i++) {
        if (mu[i] == 0) continue;
        long long q = (long long)(N / i) * mu[i];
        for (j = i; j <= N; j += i) f[j] += q;
    }
    long long best = (long long)N - 1; /* k = 1 */
    for (i = 2; i <= N; i++)
        if (f[i] < best) best = f[i];
    free(mu);
    free(f);
    free(comp);
    return (seq_t)best;
}
