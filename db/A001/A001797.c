/*
 * A001797  Coefficient of P_2(x) in the Legendre expansion of x^n.
 * https://oeis.org/A001797
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = 10*n*binomial(4*n,2*n) / ((2*n+1)*(2*n+3)*2^A000120(n)),
 *            i.e. a(n) = c_2 * D, where x^(2n) = sum_k c_{2k} P_{2k}(x), c_2
 *            = 10*n/((2n+1)(2n+3)), and D = binomial(4n,2n)/2^popcount(n) is
 *            the common denominator (the denominator of
 *            2^(2n)/binomial(4n,2n)). Evaluated via prime factorization so...
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 34,
 *            big any n
 * Helpers:   vp_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/vp_u64.h"

seq_t a(idx_t n) {
    uint64_t m4 = 4 * (uint64_t)n;
    uint64_t m2 = 2 * (uint64_t)n;
    uint64_t N = m4 + 3;
    unsigned char *comp = (unsigned char *)calloc((size_t)(N + 1), 1);
    seq_t num = 1, den = 1;
    long pc = 0;
    uint64_t t = n;
    uint64_t p, q, j;
    while (t) { pc += (long)(t & 1); t >>= 1; }

    for (p = 2; p <= N; p++) {
        long e = 0;
        if (comp[p]) continue;
        if (p * p <= N)
            for (j = p * p; j <= N; j += p) comp[j] = 1;
        for (q = p; q <= m4; q *= p) {
            e += (long)(m4 / q) - 2 * (long)(m2 / q);
            if (q > m4 / p) break;
        }
        e += vp_u64(10 * (uint64_t)n, p);
        e -= vp_u64(m2 + 1, p);
        e -= vp_u64(m2 + 3, p);
        if (p == 2) e -= pc;
        while (e > 0) { num *= (seq_t)p; e--; }
        while (e < 0) { den *= (seq_t)p; e++; }
    }
    free(comp);
    return num / den;
}
