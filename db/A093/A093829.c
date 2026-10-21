/*
 * A093829  Expansion of q * psi(q^3)^3 / psi(q) in powers of q where psi() is
 *          a Ramanujan theta function.
 * https://oeis.org/A093829
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult, easy, changed
 * Formula:   not stated
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula:  a(n) is multiplicative with 
 *   a(p^e) = (-1)^e                      if p = 2
 *   a(p^e) = 1                            if p = 3
 *   a(p^e) = 1 + e                        if p ≡ 1 (mod 6)
 *   a(p^e) = (1 + (-1)^e)/2 = 1 if e even, 0 if e odd
 *         if p ≡ 5 (mod 6)
 *
 * The sequence is computed by prime factorisation of n and multiplying
 * the contributions for each prime power.  The algorithm runs in
 * O(√n) time and is correct for all n ≥ 1.
 */

#include <stdint.h>
#include <stdbool.h>

/* seq_t and idx_t are provided by the build environment. */

seq_t a(idx_t n)
{
    if (n == 0)            /* undefined below offset, harmless */
        return 0;

    seq_t result = 1;      /* multiplicative identity */
    idx_t m = n;

    /* Treat the prime 2 separately. */
    if ((m & 1ULL) == 0) { /* n even */
        idx_t e = 0;
        while ((m & 1ULL) == 0) {
            m >>= 1;
            ++e;
        }
        if (e & 1)          /* e is odd → (-1)^e = -1 */
            result = -result;
        /* even e gives factor +1, nothing to do */
    }

    /* Factor the odd part of m. */
    for (idx_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            idx_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }

            if (p == 3) {
                /* a(p^e) = 1, no change to result */
            } else if (p % 6 == 1) {
                result *= (seq_t)(e + 1);
            } else if (p % 6 == 5) {
                if (e & 1)        /* odd exponent → factor 0 */
                    return (seq_t)0;
                /* even exponent gives factor 1, nothing to do */
            }
        }
    }

    /* If a prime factor larger than sqrt(n) remains. */
    if (m > 1) {
        idx_t p = m;
        idx_t e = 1;          /* exponent of the remaining prime is 1 */

        if (p == 3) {
            /* a(p^1) = 1, do nothing */
        } else if (p % 6 == 1) {
            result *= (seq_t)(e + 1);  /* e = 1 → factor 2 */
        } else if (p % 6 == 5) {
            /* e = 1 is odd → sequence value is 0 */
            return (seq_t)0;
        }
    }

    return result;
}
