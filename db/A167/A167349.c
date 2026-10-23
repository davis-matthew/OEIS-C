/*
 * A167349  Totally multiplicative sequence with a(p) = (p+1)^2 = p^2+2p+1 for
 *          prime p.
 * https://oeis.org/A167349
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏{p^e‖n} ((p+1)^2)^e
 * Range:     u8 n <= 7, u16 n <= 47, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1)=1; for n≥2, compute by prime factorisation. */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= (idx_t)1)
        return (seq_t)1;

    seq_t result = (seq_t)1;
    uint64_t m = (uint64_t)n;

    /* handle factor 2 separately */
    size_t e = 0;
    while (m % 2 == 0) {
        m /= 2;
        ++e;
    }
    if (e) {
        seq_t base = (seq_t)(2 + 1);          /* base = p+1 for p=2 */
        seq_t pow_base = base * base;        /* (p+1)^2 */
        for (size_t i = 0; i < e; ++i)
            result *= pow_base;
    }

    /* trial division for odd factors */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            seq_t base = (seq_t)(p + 1);
            seq_t pow_base = base * base;
            for (size_t i = 0; i < e; ++i)
                result *= pow_base;
        }
    }

    /* remaining prime factor > sqrt(original n) */
    if (m > 1) {
        seq_t base = (seq_t)(m + 1);
        seq_t pow_base = base * base;        /* exponent is 1 */
        result *= pow_base;
    }

    return result;
}
