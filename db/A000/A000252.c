/*
 * A000252  Number of invertible 2 X 2 matrices mod n.
 * https://oeis.org/A000252
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice, mult
 * Formula:   a(n) = Product_{p^e || n} (p - 1)^2*(p + 1)*p^(4e-3)
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 256, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t r = 1;
    idx_t m = n;
    idx_t p;
    for (p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            idx_t e = 0;
            idx_t k;
            while (m % p == 0) {
                m /= p;
                e++;
            }
            r *= (seq_t)(p - 1);
            r *= (seq_t)(p - 1);
            r *= (seq_t)(p + 1);
            for (k = 0; k < 4 * e - 3; k++)
                r *= (seq_t)p;
        }
    }
    if (m > 1) {
        r *= (seq_t)(m - 1);
        r *= (seq_t)(m - 1);
        r *= (seq_t)(m + 1);
        r *= (seq_t)m;
    }
    return r;
}
