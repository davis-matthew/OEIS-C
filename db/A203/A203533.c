/*
 * A203533  a(n) = A203530(n)/A000178(n-1); A000178 = (superfactorials).
 * https://oeis.org/A203533
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A203530(n)/A000178(n-1), where A203530(n) =
 *            Product_{1<=i<j<=n} (c(i)+c(j)), c = composite numbers,
 *            A000178(n-1) = Product_{k=1..n-1} k!; computed by prime-exponent
 *            bookkeeping (sieve)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 9,
 *            big any n
 * Validated: partial, pass, 11 terms through n = 11 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;

    /* find first n composites */
    unsigned long long limit = 2ULL * (unsigned long long)n + 10;
    unsigned long long *comps = (unsigned long long *)malloc(sizeof(unsigned long long) * (size_t)n);
    if (!comps) return (seq_t)0;
    while (1) {
        bool *isp = (bool *)malloc((size_t)(limit + 1));
        if (!isp) { free(comps); return (seq_t)0; }
        for (unsigned long long i = 0; i <= limit; ++i) isp[i] = (i >= 2);
        for (unsigned long long i = 2; i * i <= limit; ++i)
            if (isp[i])
                for (unsigned long long j = i * i; j <= limit; j += i) isp[j] = false;
        unsigned long long k = 0;
        for (unsigned long long i = 4; i <= limit && k < (unsigned long long)n; ++i)
            if (!isp[i]) comps[k++] = i;
        free(isp);
        if (k >= (unsigned long long)n) break;
        limit <<= 1;
    }

    unsigned long long M = 2ULL * comps[n - 1];
    if (M < (unsigned long long)n) M = (unsigned long long)n;

    /* smallest prime factor table */
    unsigned int *spf = (unsigned int *)calloc((size_t)(M + 1), sizeof(unsigned int));
    long long *ex = (long long *)calloc((size_t)(M + 1), sizeof(long long));
    if (!spf || !ex) { free(spf); free(ex); free(comps); return (seq_t)0; }
    for (unsigned long long i = 2; i <= M; ++i) {
        if (spf[i] == 0) {
            for (unsigned long long j = i; j <= M; j += i)
                if (spf[j] == 0) spf[j] = (unsigned int)i;
        }
    }

    /* numerator exponents */
    for (idx_t i = 0; i + 1 < n; ++i) {
        for (idx_t j = i + 1; j < n; ++j) {
            unsigned long long v = comps[i] + comps[j];
            while (v > 1) {
                unsigned int p = spf[v];
                ex[p]++;
                v /= p;
            }
        }
    }
    /* denominator: m appears in k! for k = m..n-1, i.e. n-m times */
    for (unsigned long long m = 2; m + 1 <= (unsigned long long)n; ++m) {
        unsigned long long v = m;
        long long cnt = (long long)((unsigned long long)n - m);
        while (v > 1) {
            unsigned int p = spf[v];
            ex[p] -= cnt;
            v /= p;
        }
    }

    seq_t result = (seq_t)1;
    for (unsigned long long p = 2; p <= M; ++p) {
        long long e = ex[p];
        for (long long t = 0; t < e; ++t)
            result *= (seq_t)p;
    }

    free(spf);
    free(ex);
    free(comps);
    return result;
}
