/*
 * A034448  usigma(n) = sum of unitary divisors of n (divisors d such that
 *          gcd(d, n/d)=1); also called UnitarySigma(n).
 * https://oeis.org/A034448
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice, mult
 * Formula:   a(n) = ∏_{p^e||n} (p^e + 1) (unitary sigma function)
 * Range:     u8 n <= 137, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Computes the unitary σ (sum of unitary divisors) of n.  The
 * function works for any n ≥ 1.  For n up to about 10 000 the
 * simple trial‑division algorithm below runs well below the
 * required 2 s per call. */
seq_t a(idx_t n)
{
    if (n <= 1)            /* a(1) = 1; a(0) is unused */
        return (seq_t)1;

    /* Use a copy so that we can modify the value while factoring. */
    uint64_t m = (uint64_t)n;
    seq_t   result = (seq_t)1;

    /* Trial‑division up to sqrt(m).  The loop terminates when
     * p*p > m; since m shrinks during the loop this guarantees
     * that all prime factors are processed. */
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p != 0)
            continue;

        /* Count the exponent of prime p. */
        int exp = 0;
        while (m % p == 0) {
            m /= p;
            ++exp;
        }

        /* Compute p^exp + 1 in seq_t. */
        seq_t term = (seq_t)1;
        for (int i = 0; i < exp; ++i)
            term *= (seq_t)p;
        term += (seq_t)1;

        result *= term;
    }

    /* If a prime factor larger than sqrt(original n) remains. */
    if (m > 1) {
        seq_t term = (seq_t)m + (seq_t)1;
        result *= term;
    }

    return result;
}
