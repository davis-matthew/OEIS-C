/*
 * A166643  Totally multiplicative sequence with a(p) = 3*(p+1) for prime p.
 * https://oeis.org/A166643
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = product over prime powers p^e in n of (3*(p+1))^e
 * Range:     u8 n <= 7, u16 n <= 47, u32 n <= 1535, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    uint64_t nn = (uint64_t)n;
    seq_t result = (seq_t)1;

    /* factor 2 separately */
    uint64_t exp = 0;
    while ((nn & 1ULL) == 0) {
        nn >>= 1;
        exp++;
    }
    if (exp) {
        seq_t factor = (seq_t)3 * ((seq_t)2 + (seq_t)1);
        for (uint64_t e = 0; e < exp; ++e) result *= factor;
    }

    /* trial division by odd numbers up to sqrt(nn) */
    for (uint64_t p = 3; p * p <= nn; p += 2) {
        exp = 0;
        while (nn % p == 0) {
            nn /= p;
            exp++;
        }
        if (exp) {
            seq_t factor = (seq_t)3 * ((seq_t)p + (seq_t)1);
            for (uint64_t e = 0; e < exp; ++e) result *= factor;
        }
    }

    /* if a prime factor > sqrt(original n) remains */
    if (nn > 1) {
        seq_t factor = (seq_t)3 * ((seq_t)nn + (seq_t)1);
        result *= factor;
    }

    return result;
}
