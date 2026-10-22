/*
 * A000056  Order of the group SL(2,Z_n).
 * https://oeis.org/A000056
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   Multiplicative with a(p^e) = (p^2 - 1)*p^(3e-2)
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1000, u64 n <= 1000,
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
            int e = 0;
            int i;
            seq_t f;
            while (m % p == 0) {
                m /= p;
                e++;
            }
            f = (seq_t)p * (seq_t)p - 1;
            for (i = 0; i < 3 * e - 2; i++)
                f *= (seq_t)p;
            r *= f;
        }
    }
    if (m > 1) {
        seq_t f = (seq_t)m * (seq_t)m - 1;
        f *= (seq_t)m;
        r *= f;
    }
    return r;
}
