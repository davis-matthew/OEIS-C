/*
 * A167328  Totally multiplicative sequence with a(p) = 9*(p+3) for prime p.
 * https://oeis.org/A167328
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Π_{p^e‖n} (9·(p+3))^e (a(p) = 9·(p+3) for prime p;
 *            multiplicative on prime powers)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 63, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Computes the A167328 sequence value at index n. */
seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    seq_t result = (seq_t)1;
    uint64_t m = n;

    for (uint64_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            idx_t exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            seq_t base = (seq_t)9 * ((seq_t)p + (seq_t)3);
            seq_t pow = (seq_t)1;
            for (idx_t i = 0; i < exp; ++i)
                pow *= base;
            result *= pow;
        }
    }

    if (m > 1) {  /* remaining prime factor of exponent 1 */
        seq_t base = (seq_t)9 * ((seq_t)m + (seq_t)3);
        result *= base;
    }

    return result;
}
