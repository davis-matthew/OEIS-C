/*
 * A005158  Number of alternating sign n X n matrices invariant under a
 *          half-turn.
 * https://oeis.org/A005158
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, more
 * Formula:   a(n) = a(n-1) * (1 + [n even]/3) * C(3*(n\2), n\2) / C(2*(n\2),
 *            n\2), a(1) = 1. Unrolled: a(n) = (4/3)^(n\2) * prod_{i=2..n}
 *            r(i\2), where r(k) = (3k)! k! / ((2k)!)^2. The code adds up the
 *            exponent of each prime with Legendre's formula, then multiplies
 *            the primes together, so it never divides
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static long long leg(long long N, long long p) {
    long long s = 0;
    while (N) { N /= p; s += N; }
    return s;
}

seq_t a(idx_t n) {
    long long m = (long long)(n / 2);
    long long K = m;               /* number of even i in 2..n */
    long long lim = 3 * m;
    seq_t result = 1;
    char *comp;
    long long p, k, e, t;

    if (n <= 1) return 1;

    comp = (char *)calloc((size_t)(lim + 2), 1);
    for (p = 2; p <= lim; p++) {
        if (comp[p]) continue;
        for (t = p * p; t <= lim; t += p) comp[t] = 1;
        e = 0;
        for (k = 1; k <= m; k++) {
            long long w = 2;
            long long v;
            if (k == m && n % 2 == 0) w = 1;
            if (3 * k < p) continue;
            v = leg(3 * k, p) + leg(k, p) - 2 * leg(2 * k, p);
            e += w * v;
        }
        if (p == 2) e += 2 * K;
        if (p == 3) e -= K;
        while (e-- > 0) result *= (seq_t)p;
    }
    free(comp);
    return result;
}
