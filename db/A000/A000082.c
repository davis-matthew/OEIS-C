/*
 * A000082  a(n) = n^2*Product_{p|n} (1 + 1/p).
 * https://oeis.org/A000082
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice, mult
 * Formula:   a(n) = n^2*Product_{p|n} (1 + 1/p); multiplicative with a(p^e) =
 *            p^(2*e-1)*(p+1)
 * Range:     u8 n <= 11, u16 n <= 179, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t r = 1;
    uint64_t m = (uint64_t)n;
    uint64_t p;
    for (p = 2; p * p <= m; p += (p == 2 ? 1 : 2)) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) {
                m /= p;
                e++;
            }
            seq_t t = (seq_t)p + 1;
            int i;
            for (i = 0; i < 2 * e - 1; i++)
                t *= (seq_t)p;
            r *= t;
        }
    }
    if (m > 1) {
        r *= (seq_t)m * ((seq_t)m + 1);
    }
    return r;
}
