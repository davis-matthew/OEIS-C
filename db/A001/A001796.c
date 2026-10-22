/*
 * A001796  Coefficient of P_1(x) in the Legendre expansion of x^n.
 * https://oeis.org/A001796
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = numerator(3*binomial(2*n+1/2, n)/(2*n+3)) =
 *            numerator(3*(4n+1)!!/((2n+3)!! * 2^n * n!)), computed exactly
 *            from the prime factorization of the reduced fraction
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 34,
 *            big any n
 * Helpers:   a001796_vfact
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/a001796_vfact.h"

seq_t a(idx_t n) {
    unsigned long long N = (unsigned long long)n;
    unsigned long long lim = 4 * N + 3;
    unsigned char *comp = (unsigned char *)malloc((size_t)(lim + 1));
    memset(comp, 0, (size_t)(lim + 1));
    seq_t r = 1;
    for (unsigned long long p = 3; p <= lim; p += 2) {
        if (comp[p]) continue;
        for (unsigned long long q = p * p; q <= lim; q += 2 * p) comp[q] = 1;
        /* exponent of p in (4n+1)!! / (2n+3)!!, times 3, divided by n! */
        long long e = (long long)a001796_vfact(4 * N + 1, p)
                    - (long long)a001796_vfact(2 * N, p)
                    - (long long)a001796_vfact(2 * N + 3, p)
                    + (long long)a001796_vfact(N + 1, p)
                    - (long long)a001796_vfact(N, p);
        if (p == 3) e += 1;
        for (long long i = 0; i < e; i++) r *= (seq_t)p;
    }
    free(comp);
    return r;
}
