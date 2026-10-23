/*
 * A035154  a(n) = Sum_{d|n} Kronecker(-36, d).
 * https://oeis.org/A035154
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Sum_{d|n} Kronecker(-36, d) Equivalent multiplicative
 *            representation: - a(2^e) = a(3^e) = 1 - for p ≡ 1 (mod 4) :
 *            a(p^e) = e + 1 - for p ≡ 3 (mod 4) : a(p^e) = (1 + (-1)^e)/2
 *            Thus a(n) is multiplicative and can be computed by prime
 *            factorisation
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* n is ≥ 1 */
    seq_t result = 1;          /* multiplicative accumulator   */
    uint64_t m = (uint64_t)n;  /* work copy for factorisation */

    /* count factor 2 */
    while ((m & 1ULL) == 0) {
        m >>= 1;               /* divide by 2, exponent irrelevant   */
    }

    /* count factor 3 */
    while (m % 3ULL == 0) {
        m /= 3ULL;              /* divide by 3, exponent irrelevant   */
    }

    /* factor remaining primes using 6k±1 step */
    for (uint64_t i = 5ULL; (unsigned __int128)i * i <= m; i += 6ULL) {
        /* check i   (6k-1) */
        if (m % i == 0) {
            uint64_t e = 0;
            while (m % i == 0) {
                m /= i;
                ++e;
            }
            if (i % 4ULL == 1ULL) {
                result *= (seq_t)(e + 1);        /* p ≡ 1 (mod 4)  */
            } else {                          /* p ≡ 3 (mod 4)  */
                if (e & 1ULL) return 0;          /* odd exponent → 0  */
                /* even exponent → multiplier 1 → no change */
            }
        }

        /* check i+2 (6k+1) */
        uint64_t j = i + 2ULL;
        if ((unsigned __int128)j * j > m) continue;
        if (m % j == 0) {
            uint64_t e = 0;
            while (m % j == 0) {
                m /= j;
                ++e;
            }
            if (j % 4ULL == 1ULL) {
                result *= (seq_t)(e + 1);        /* p ≡ 1 (mod 4)  */
            } else {                          /* p ≡ 3 (mod 4)  */
                if (e & 1ULL) return 0;          /* odd exponent → 0  */
            }
        }
    }

    /* if m > 1, it is a prime factor > sqrt(original n) */
    if (m > 1ULL) {
        if (m % 4ULL == 1ULL) {
            result *= (seq_t)2;          /* e = 1 → e+1 = 2   */
        } else {                        /* m ≡ 3 (mod 4) */
            return 0;                    /* odd exponent (1)  */
        }
    }

    return result;
}
