/*
 * A166661  Totally multiplicative sequence with a(p) = 3p+1 for prime p.
 * https://oeis.org/A166661
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p^e‖n} (3p+1)^e (totally multiplicative, a(p)=3p+1)
 * Range:     u8 n <= 7, u16 n <= 63, u32 n <= 4095, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute the A166661 sequence value for a given index n.
 * The function works for any seq_t representation provided by the build.
 */
seq_t a(idx_t n)
{
    /* a(1) is defined to be 1; indices 0 and below are outside the sequence. */
    if (n <= 1)
        return (seq_t)1;

    seq_t result = (seq_t)1;
    uint64_t m = (uint64_t)n;            /* work with 64‑bit arithmetic for factorisation */

    /* Handle factor 2 separately for speed and to keep the loop odd-only. */
    if ((m & 1ULL) == 0) {
        uint64_t e = 0;
        while ((m & 1ULL) == 0) {
            m >>= 1;
            ++e;
        }
        /* (3·2+1)^e = 7^e */
        seq_t term = (seq_t)1;
        seq_t base = (seq_t)7;
        for (uint64_t i = 0; i < e; ++i)
            term *= base;
        result *= term;
    }

    /* Test odd candidates up to sqrt(m). */
    for (uint64_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            seq_t term = (seq_t)1;
            seq_t base = (seq_t)(3ULL * p + 1);
            for (uint64_t i = 0; i < e; ++i)
                term *= base;
            result *= term;
        }
    }

    /* If a prime factor greater than sqrt(original n) remains, it appears once. */
    if (m > 1) {
        seq_t base = (seq_t)(3ULL * m + 1);
        result *= base;
    }

    return result;
}
