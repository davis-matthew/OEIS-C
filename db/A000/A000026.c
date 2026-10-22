/*
 * A000026  Mosaic numbers or multiplicative projection of n: if n = Product
 *          (p_j^k_j) then a(n) = Product (p_j * k_j).
 * https://oeis.org/A000026
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice, mult
 * Formula:   multiplicative with a(p^e) = p*e, i.e. n = Product (p_j^k_j) ->
 *            a(n) = Product (p_j * k_j)
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
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
    for (p = 2; p <= m / p; p++) {
        if (m % p == 0) {
            idx_t e = 0;
            while (m % p == 0) {
                m /= p;
                e++;
            }
            r *= (seq_t)p;
            r *= (seq_t)e;
        }
    }
    if (m > 1)
        r *= (seq_t)m;
    return r;
}
