/*
 * A006635  a(n) = 4*binomial(4*n+11, n)/(n+4).
 * https://oeis.org/A006635
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*binomial(4*n+11, n)/(n+4) = 4*(4n+11)! / (n! * (3n+11)!
 *            * (n+4)), evaluated by prime factorization (Legendre's formula)
 *            so intermediates never exceed the result
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t leg(uint64_t m, uint64_t p) {
    uint64_t e = 0;
    while (m) { m /= p; e += m; }
    return e;
}

seq_t a(idx_t n) {
    uint64_t top = 4 * (uint64_t)n + 11;
    unsigned char *comp = (unsigned char *)calloc((size_t)top + 1, 1);
    seq_t r = 1;
    uint64_t chunk = 1;
    uint64_t p, q;
    for (p = 2; p <= top; p++) {
        uint64_t e, m, k;
        if (comp[p]) continue;
        for (q = p * p; q <= top; q += p) comp[q] = 1;
        e = leg(top, p) - leg(n, p) - leg(3 * (uint64_t)n + 11, p);
        if (p == 2) e += 2;
        m = (uint64_t)n + 4;
        while (m % p == 0) { m /= p; e--; }
        for (k = 0; k < e; k++) {
            if (chunk > (UINT64_C(1) << 32) / p) {
                r *= (idx_t)chunk;
                chunk = 1;
            }
            chunk *= p;
        }
    }
    r *= (idx_t)chunk;
    free(comp);
    return r;
}
