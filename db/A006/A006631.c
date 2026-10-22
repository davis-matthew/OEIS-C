/*
 * A006631  From generalized Catalan numbers.
 * https://oeis.org/A006631
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*binomial(3*n + 8, n)/(3*n + 8) =
 *            8*(3n+7)!/(n!*(2n+8)!), computed as a product of prime powers
 *            (Legendre exponents), so every partial product divides the
 *            result
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 24, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static uint64_t leg_exp(uint64_t m, uint64_t p) {
    uint64_t s = 0;
    while (m) {
        m /= p;
        s += m;
    }
    return s;
}

seq_t a(idx_t n) {
    uint64_t top = 3 * (uint64_t)n + 7;
    uint64_t lim = top + 1;
    unsigned char *comp = (unsigned char *)calloc((size_t)lim + 1, 1);
    seq_t r = 1;
    uint64_t p, q, i;
    for (p = 2; p <= top; p++) {
        if (comp[p]) continue;
        for (q = p * p; q <= top; q += p) comp[q] = 1;
        {
            uint64_t e = leg_exp(top, p) - leg_exp((uint64_t)n, p)
                         - leg_exp(2 * (uint64_t)n + 8, p);
            if (p == 2) e += 3;
            for (i = 0; i < e; i++) r *= (seq_t)p;
        }
    }
    free(comp);
    return r;
}
