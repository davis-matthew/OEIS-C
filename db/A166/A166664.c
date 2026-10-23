/*
 * A166664  Totally multiplicative sequence with a(p) = 6p+1 for prime p.
 * https://oeis.org/A166664
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Π_{p^e||n} (6p+1)^e (multiplicative with a(p^e)=(6p+1)^e)
 * Range:     u8 n <= 7, u16 n <= 31, u32 n <= 511, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;   /* a(1)=1 */

    seq_t result = (seq_t)1;
    idx_t m = n;

    /* factor 2 separately */
    int e = 0;
    while ((m & 1ULL) == 0) {
        m >>= 1;
        ++e;
    }
    if (e) {
        seq_t base = (seq_t)6 * (seq_t)2 + (seq_t)1;
        seq_t powv = (seq_t)1;
        for (int i = 0; i < e; ++i) powv *= base;
        result *= powv;
    }

    /* odd prime factors */
    for (idx_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            seq_t base = (seq_t)6 * (seq_t)p + (seq_t)1;
            seq_t powv = (seq_t)1;
            for (int i = 0; i < e; ++i) powv *= base;
            result *= powv;
        }
    }

    /* remaining prime factor > sqrt(n) */
    if (m > 1) {
        seq_t base = (seq_t)6 * (seq_t)m + (seq_t)1;
        result *= base;   /* exponent is 1 */
    }

    return result;
}
