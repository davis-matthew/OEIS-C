/*
 * A166671  Totally multiplicative sequence with a(p) = 7p-2 for prime p.
 * https://oeis.org/A166671
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p^e||n} (7p-2)^e
 * Range:     u8 n <= 7, u16 n <= 31, u32 n <= 511, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* a(1) = 1 by definition */
    if (n == 1)
        return (seq_t)1;

    uint64_t m = (uint64_t)n;     /* use 64‑bit arithmetic for factorization   */
    seq_t res = 1;                /* product of (7p-2)^e for each prime power */

    /* trial division up to sqrt(m) */
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            seq_t base = (seq_t)p * 7 - 2;   /* 7p-2   */
            seq_t term = 1;
            while (e--) term *= base;       /* (7p-2)^e   */
            res *= term;                    /* multiply into product */
        }
    }

    /* if a prime factor > sqrt(original n) remains, it appears once */
    if (m > 1) {
        seq_t base = (seq_t)m * 7 - 2;   /* exponent e = 1   */
        res *= base;
    }

    return res;
}
