/*
 * A005789  3-dimensional Catalan numbers.
 * https://oeis.org/A005789
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, walk, nice
 * Formula:   a(n) = 2*(3*n)!/(n!*(n+1)!*(n+2)!), evaluated via Legendre prime
 *            exponents
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static long long vfact(uint64_t m, uint64_t p) {
    long long e = 0;
    while (m > 0) {
        m /= p;
        e += (long long)m;
    }
    return e;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    uint64_t lim = 3 * (uint64_t)n;
    unsigned char *comp = (unsigned char *)calloc((size_t)lim + 1, 1);
    seq_t r = (seq_t)1;
    for (uint64_t p = 2; p <= lim; p++) {
        if (comp[p]) continue;
        for (uint64_t q = p * p; q <= lim; q += p) comp[q] = 1;
        long long e = vfact(lim, p) - vfact(n, p) - vfact(n + 1, p) - vfact(n + 2, p);
        if (p == 2) e += 1;
        for (long long i = 0; i < e; i++) r *= (seq_t)p;
    }
    free(comp);
    return r;
}
