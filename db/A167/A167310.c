/*
 * A167310  Totally multiplicative sequence with a(p) = 9*(p+2) for prime p.
 * https://oeis.org/A167310
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏_{p^e‖n} (9·(p+2))^e (multiplicative definition)
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 127, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t m = (uint64_t)n;      /* work on 64‑bit unsigned integer */
    seq_t   result = 1;            /* product of the contributions */

    /* Handle the factor 2 separately for efficiency */
    while ((m & 1ULL) == 0ULL) {
        seq_t base = (seq_t)9 * ((seq_t)2 + 2);   /* 9·(2+2) = 36 */
        result *= base;                          /* multiply once per exponent */
        m >>= 1;                                 /* divide by 2 */
    }

    /* Trial division by odd numbers up to sqrt(m) */
    for (uint64_t p = 3ULL; p <= m / p; p += 2ULL) {
        while (m % p == 0ULL) {
            seq_t base = (seq_t)9 * ((seq_t)p + 2);  /* 9·(p+2) */
            result *= base;
            m /= p;
        }
    }

    /* If anything remains, it is a prime factor > 1 */
    if (m > 1ULL) {
        seq_t base = (seq_t)9 * ((seq_t)m + 2);
        result *= base;
    }

    return result;
}
