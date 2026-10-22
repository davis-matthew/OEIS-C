/*
 * A000010  Euler totient function phi(n): count numbers <= n and prime to n.
 * https://oeis.org/A000010
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, core, nonn, mult, nice, hear, changed
 * Formula:   a(n) = n*Product_{distinct primes p dividing n} (1 - 1/p);
 *            multiplicative with a(p^e) = (p - 1)*p^(e-1)
 * Range:     u8 n <= 256, u16 n <= 9868, u32 n <= 9868, u64 n <= 9868,
 *            u128 n <= 9868, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t result = 1;
    idx_t m = n;
    idx_t p;
    for (p = 2; p <= m / p; p++) {
        if (m % p == 0) {
            m /= p;
            result *= (seq_t)(p - 1);
            while (m % p == 0) {
                m /= p;
                result *= (seq_t)p;
            }
        }
    }
    if (m > 1)
        result *= (seq_t)(m - 1);
    return result;
}
