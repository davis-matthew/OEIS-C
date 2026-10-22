/*
 * A007857  Number of independent sets in rooted plane trees on n nodes.
 * https://oeis.org/A007857
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n+1) = (2/(n+1))*C(3*n, n) - (1/(n+1))*C(2*n, n) = A007226(n)
 *            - A000108(n); both terms are built from their prime
 *            factorizations (Legendre), using multiplication only
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   powmul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/powmul.h"

static long long leg(long long N, long long p) {
    long long e = 0;
    while (N > 0) { N /= p; e += N; }
    return e;
}

seq_t a(idx_t n) {
    long long m, lim, i, j;
    char *comp;
    seq_t b = 1, c = 1;
    if (n == 0) return 0;
    m = (long long)(n - 1);
    lim = 3 * m + 2;
    comp = (char *)calloc((size_t)(lim + 1), 1);
    for (i = 2; i <= lim; i++) {
        long long eb, ec;
        if (comp[i]) continue;
        for (j = i * i; j <= lim; j += i) comp[j] = 1;
        /* b = 2*(3m)! / ((m+1)! (2m)!) */
        eb = leg(3 * m, i) - leg(m + 1, i) - leg(2 * m, i) + (i == 2 ? 1 : 0);
        /* c = (2m)! / (m! (m+1)!) */
        ec = leg(2 * m, i) - leg(m, i) - leg(m + 1, i);
        if (eb > 0) b = powmul(b, (seq_t)i, eb);
        if (ec > 0) c = powmul(c, (seq_t)i, ec);
    }
    free(comp);
    return b - c;
}
