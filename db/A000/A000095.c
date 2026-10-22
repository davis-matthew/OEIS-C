/*
 * A000095  Number of fixed points of Gamma_0 (n) of type i.
 * https://oeis.org/A000095
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) is multiplicative with a(2) = 2, a(2^e) = 0 if e>1, a(p^e)
 *            = 2 if p == 1 mod 4, a(p^e) = 0 if p == 3 mod 4
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
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
    if (m % 2 == 0) {
        m /= 2;
        if (m % 2 == 0) return (seq_t)0;
        r *= 2;
    }
    for (p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            if (p % 4 == 3) return (seq_t)0;
            while (m % p == 0) m /= p;
            r *= 2;
        }
    }
    if (m > 1) {
        if (m % 4 == 3) return (seq_t)0;
        r *= 2;
    }
    return r;
}
