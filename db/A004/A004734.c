/*
 * A004734  Numerator of average distance traveled by n-dimensional fly.
 * https://oeis.org/A004734
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   for n = 2m, a(n) = 2^(2m+2-A000120(m)) (numerator of
 *            4^(m+1)/((2m+1)*binomial(2m,m))); for n = 2m-1, a(n) = odd part
 *            of binomial(2m,m) (numerator of binomial(2m,m)/2^(2m-1)), built
 *            from Legendre/Kummer prime exponents so intermediates never
 *            exceed a(n)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   is_odd_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_odd_prime_small.h"

seq_t a(idx_t n) {
    seq_t r = 1;
    if (n % 2 == 0) {
        idx_t m = n / 2, t = m, pc = 0, e, i;
        while (t) { pc += t & 1; t >>= 1; }
        e = 2 * m + 2 - pc;
        for (i = 0; i < e; i++) r *= 2;
        return r;
    } else {
        idx_t m = (n + 1) / 2, p;
        for (p = 3; p <= 2 * m; p += 2) {
            idx_t e = 0, pk, k;
            if (!is_odd_prime_small(p)) continue;
            for (pk = p; pk <= 2 * m; pk *= p) {
                e += (2 * m) / pk - 2 * (m / pk);
                if (pk > (2 * m) / p) break;
            }
            for (k = 0; k < e; k++) r *= (seq_t)p;
        }
        return r;
    }
}
