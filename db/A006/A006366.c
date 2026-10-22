/*
 * A006366  Number of cyclically symmetric plane partitions in the n-cube;
 *          also number of 2n X 2n half-turn symmetric alternating sign
 *          matrices divided by number of n X n alternating sign matrices.
 * https://oeis.org/A006366
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = Product_{i=1..n} (((3*i-1)/(3*i-2))*Product_{j=i..n}
 *            (n+i+j-1)/(2*i+j-1)), evaluated exactly by accumulating the
 *            exponent of each integer factor, reducing to prime exponents
 *            with a smallest-prime-factor sieve, then multiplying
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 12, u128 n <= 18,
 *            big any n
 * Helpers:   a6366_ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/a6366_ipow.h"

seq_t a(idx_t n) {
    if (n == 0) return 1;
    long long N = (long long)n;
    long long M = 3 * N + 2;
    long long *d = (long long *)calloc((size_t)(M + 1), sizeof(long long));
    long long *e = (long long *)calloc((size_t)(M + 1), sizeof(long long));
    long long *spf = (long long *)calloc((size_t)(M + 1), sizeof(long long));
    long long i, m;

    for (i = 1; i <= N; i++) {
        /* factor (3i-1)/(3i-2) */
        e[3 * i - 1] += 1;
        e[3 * i - 2] -= 1;
        /* numerators n+i+j-1 for j=i..n : range [n+2i-1, 2n+i-1] */
        d[N + 2 * i - 1] += 1;
        d[2 * N + i] -= 1;
        /* denominators 2i+j-1 for j=i..n : range [3i-1, 2i+n-1] */
        d[3 * i - 1] -= 1;
        d[2 * i + N] += 1;
    }
    {
        long long run = 0;
        for (m = 0; m <= M; m++) {
            run += d[m];
            e[m] += run;
        }
    }

    for (m = 2; m <= M; m++) {
        if (spf[m] == 0) {
            long long k;
            for (k = m; k <= M; k += m)
                if (spf[k] == 0) spf[k] = m;
        }
    }

    for (m = M; m >= 2; m--) {
        if (e[m] != 0 && spf[m] != m) {
            long long p = spf[m];
            e[p] += e[m];
            e[m / p] += e[m];
            e[m] = 0;
        }
    }

    seq_t r = 1;
    for (m = 2; m <= M; m++) {
        if (e[m] > 0) r *= a6366_ipow((seq_t)m, e[m]);
    }

    free(d);
    free(e);
    free(spf);
    return r;
}
