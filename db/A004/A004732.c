/*
 * A004732  Numerator of n!!/(n+3)!!.
 * https://oeis.org/A004732
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(2m) = 2^(2m - A000120(m)); a(2m+1) = A000265(A000108(m+1)) =
 *            (2m+1)!!/(m+2)! with the 2s removed, built from prime exponents
 *            v_p((2m+1)!) - v_p(m!) - v_p((m+2)!) for odd primes p
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   legendre
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/legendre.h"

seq_t a(idx_t n) {
    idx_t m = n / 2;
    if (n % 2 == 0) {
        idx_t pc = 0, t = m;
        while (t) {
            pc += t & 1;
            t >>= 1;
        }
        idx_t e = 2 * m - pc;
        seq_t r = 1;
        for (idx_t i = 0; i < e; i++) r *= 2;
        return r;
    }
    {
        idx_t lim = 2 * m + 1;
        char *comp = (char *)calloc((size_t)lim + 1, 1);
        seq_t r = 1;
        idx_t p, q;
        for (p = 2; p <= lim; p++) {
            if (comp[p]) continue;
            for (q = p * p; q <= lim; q += p) comp[q] = 1;
            if (p == 2) continue;
            {
                uint64_t e = legendre(lim, p) - legendre(m, p) - legendre(m + 2, p);
                uint64_t k;
                for (k = 0; k < e; k++) r *= (seq_t)p;
            }
        }
        free(comp);
        return r;
    }
}
