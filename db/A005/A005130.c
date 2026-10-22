/*
 * A005130  Robbins numbers: a(n) = Product_{k=0..n-1} (3k+1)!/(n+k)!; also
 *          the number of descending plane partitions whose parts do not
 *          exceed n; also the number of n X n alternating sign matrices
 *          (ASM's).
 * https://oeis.org/A005130
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, core
 * Formula:   a(n) = Product_{k=0..n-1} (3k+1)!/(n+k)!, evaluated via exact
 *            prime-exponent counting
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Helpers:   a005130_powp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/a005130_powp.h"

seq_t a(idx_t n) {
    if (n <= 1) return 1;
    long long nn = (long long)n;
    long long N = 3 * nn - 2;
    long long *spf = (long long *)calloc((size_t)(N + 1), sizeof(long long));
    long long *ex = (long long *)calloc((size_t)(N + 1), sizeof(long long));
    for (long long i = 2; i <= N; i++) {
        if (spf[i] == 0) {
            for (long long j = i; j <= N; j += i)
                if (spf[j] == 0) spf[j] = i;
        }
    }
    for (long long m = 2; m <= N; m++) {
        /* multiplicity of m in numerator: #{k : 3k+1 >= m} */
        long long lo1 = (m - 1 + 2) / 3;
        long long c1 = nn - lo1;
        if (c1 < 0) c1 = 0;
        /* multiplicity of m in denominator: #{k : n+k >= m} */
        long long lo2 = m - nn;
        if (lo2 < 0) lo2 = 0;
        long long c2 = nn - lo2;
        if (c2 < 0) c2 = 0;
        long long e = c1 - c2;
        if (e == 0) continue;
        long long x = m;
        while (x > 1) {
            long long p = spf[x];
            while (x % p == 0) {
                x /= p;
                ex[p] += e;
            }
        }
    }
    seq_t r = 1;
    for (long long p = 2; p <= N; p++) {
        if (ex[p] > 0) r *= a005130_powp((seq_t)p, ex[p]);
    }
    free(spf);
    free(ex);
    return r;
}
