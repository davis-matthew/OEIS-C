/*
 * A007556  Number of 8-ary trees with n vertices.
 * https://oeis.org/A007556
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, eigen
 * Formula:   a(n) = binomial(8*n, n)/(7*n+1) = (8n)! / (n! * (7n+1)!),
 *            evaluated as a product of prime powers p^e with e = v_p((8n)!) -
 *            v_p(n!) - v_p((7n+1)!) (Legendre), so no intermediate exceeds
 *            a(n)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint64_t leg(uint64_t m, uint64_t p) {
    uint64_t s = 0;
    while (m) {
        m /= p;
        s += m;
    }
    return s;
}

seq_t a(idx_t n) {
    seq_t r = 1;
    if (n == 0) return r;
    uint64_t lim = 8 * (uint64_t)n;
    unsigned char *comp = (unsigned char *)calloc((size_t)lim + 1, 1);
    uint64_t p, q;
    for (p = 2; p <= lim; p++) {
        if (comp[p]) continue;
        if (p * p <= lim)
            for (q = p * p; q <= lim; q += p) comp[q] = 1;
        uint64_t e = leg(lim, p) - leg((uint64_t)n, p) - leg(7 * (uint64_t)n + 1, p);
        while (e--) r *= (seq_t)p;
    }
    free(comp);
    return r;
}
