/*
 * A004735  Denominator of average distance traveled by n-dimensional fly.
 * https://oeis.org/A004735
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   n odd, n = 2m-1: a(n) = 2^(2m-1-A000120(m)) = denominator of
 *            2*binomial(2m,m)/4^m; n even, n = 2m: a(n) = odd part of
 *            (2m+1)*binomial(2m,m) = (2m+1)!/(m!)^2 with all factors 2
 *            removed, computed as Product_{odd primes p <= 2m+1}
 *            p^(v_p((2m+1)!) - 2*v_p(m!))
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   legendre
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/legendre.h"

seq_t a(idx_t n) {
    seq_t r = 1;
    if (n % 2 == 1) {
        uint64_t m = (n + 1) / 2;
        uint64_t pc = 0, t = m;
        while (t) { pc += t & 1; t >>= 1; }
        uint64_t e = 2 * m - 1 - pc;
        for (uint64_t i = 0; i < e; i++) r *= 2;
        return r;
    } else {
        uint64_t m = n / 2;
        uint64_t N = 2 * m + 1;
        char *comp = (char *)calloc(N + 1, 1);
        for (uint64_t p = 3; p <= N; p += 2) {
            if (comp[p]) continue;
            for (uint64_t q = p * p; q <= N; q += 2 * p) comp[q] = 1;
            uint64_t e = legendre(N, p) - 2 * legendre(m, p);
            for (uint64_t i = 0; i < e; i++) r *= (seq_t)p;
        }
        free(comp);
        return r;
    }
}
