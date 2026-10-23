/*
 * A167309  Totally multiplicative sequence with a(p) = 8*(p+2) for prime p.
 * https://oeis.org/A167309
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏_{p^e || n} (8*(p+2))^e (totally multiplicative,
 *            a(p)=8*(p+2))
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 127, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* a(1) = 1;  for n<1 behaviour is irrelevant for the OEIS offset. */
    if (n <= 1ULL) return (seq_t)1;

    uint64_t m = (uint64_t)n;          /* factorization works on 64‑bit values */
    seq_t result = (seq_t)1;

    while (m > 1ULL) {
        /* Find the smallest prime factor of m. */
        uint64_t p = 2ULL;
        while (p * p <= m && m % p != 0ULL) {
            p = (p == 2ULL) ? 3ULL : p + 2ULL;   /* skip even numbers after 2 */
        }
        if (p * p > m) {
            /* m is prime. */
            p = m;
        }

        /* Count exponent of this prime factor. */
        unsigned int exp = 0;
        while (m % p == 0ULL) {
            m /= p;
            ++exp;
        }

        /* Factor contribution for this prime. */
        seq_t factor = (seq_t)8 * ((seq_t)p + (seq_t)2);

        /* Multiply the contribution exp times. */
        for (unsigned int i = 0; i < exp; ++i) {
            result *= factor;
        }
    }
    return result;
}
