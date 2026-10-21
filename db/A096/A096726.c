/*
 * A096726  Expansion of eta(q^3)^10 / (eta(q) * eta(q^9))^3 in powers of q.
 * https://oeis.org/A096726
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3 * b(n) where * b(1) = 1, * b(3^e) = 4 (for any e ≥ 1),
 *            * b(p^e) = 1 + p + p^2 + … + p^e for prime p ≠ 3
 * Range:     u8 n <= 39, u16 n <= 6599, u32 n <= 9885, u64 n <= 9885,
 *            u128 n <= 9885, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;            /* sequence offset */

    uint64_t m = (uint64_t)n;              /* work on a 64‑bit copy */
    seq_t prod = (seq_t)1;

    /* factor 2 */
    if ((m & 1U) == 0) {
        unsigned e = 0;
        while ((m & 1U) == 0) {
            m >>= 1U;
            ++e;
        }
        seq_t sum  = (seq_t)1;
        seq_t term = (seq_t)1;
        for (unsigned i = 0; i < e; ++i) {
            term <<= 1U;           /* multiply by 2 */
            sum  += term;
        }
        prod *= sum;
    }

    /* factor odd primes */
    for (uint64_t p = 3; p * p <= m; p += 2U) {
        if (m % p == 0) {
            unsigned e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            if (p == 3U) {
                prod *= (seq_t)4;   /* 1 + 3*(e>0) = 4 */
            } else {
                seq_t sum  = (seq_t)1;
                seq_t term = (seq_t)1;
                for (unsigned i = 0; i < e; ++i) {
                    term = (seq_t)term * (seq_t)p;
                    sum  = sum + term;
                }
                prod *= sum;
            }
        }
    }

    /* remaining prime factor > 1 */
    if (m > 1U) {
        if (m == 3U) {
            prod *= (seq_t)4;
        } else {
            prod *= ((seq_t)1 + (seq_t)m);   /* sum 1 + p for e = 1 */
        }
    }

    return (prod * (seq_t)3);   /* a(n) = 3 * b(n) */
}
