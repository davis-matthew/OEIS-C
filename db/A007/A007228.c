/*
 * A007228  a(n) = 3*binomial(4*n,n)/(n+1).
 * https://oeis.org/A007228
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*binomial(4*n,n)/(n+1), computed as a product of prime
 *            powers (Legendre's formula)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    uint64_t lim = 4 * (uint64_t)n;
    if (lim < 3) lim = 3;
    char *comp = (char *)calloc((size_t)lim + 1, 1);
    seq_t r = 1;
    uint64_t p, q, i;
    for (p = 2; p <= lim; p++) {
        long long e;
        uint64_t m;
        if (comp[p]) continue;
        for (i = p * p; i <= lim; i += p) comp[i] = 1;
        e = 0;
        for (q = p; q <= 4 * (uint64_t)n; q *= p) {
            e += (long long)((4 * (uint64_t)n) / q);
            e -= (long long)((uint64_t)n / q);
            e -= (long long)((3 * (uint64_t)n) / q);
            if (q > (4 * (uint64_t)n) / p) break;
        }
        m = (uint64_t)n + 1;
        while (m % p == 0) { m /= p; e--; }
        if (p == 3) e++;
        while (e-- > 0) r *= (seq_t)p;
    }
    free(comp);
    return r;
}
