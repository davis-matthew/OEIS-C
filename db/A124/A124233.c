/*
 * A124233  Expansion of psi(q) * phi(-q^10) * chi(-q^5) / chi(-q^2) in powers
 *          of q where phi(), psi(), chi() are Ramanujan theta functions.
 * https://oeis.org/A124233
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A124233: multiplicative function defined by
 * a(0) = 1
 * a(p^e) = 1            if p == 2 or p == 5
 * a(p^e) = e + 1        if p mod 20 ∈ {1, 3, 7, 9}
 * a(p^e) = (1 + (-1)^e)/2  if p mod 20 ∈ {11, 13, 17, 19}
 * a(n) = product over prime powers.
 * a is multiplicative.  Here we implement it by
 * prime factorisation of n and multiplying the
 * corresponding local factors.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Handle the zeroth term */
    if (n == 0) {
        return (seq_t)1;
    }

    /* local copy of n for factorisation */
    uint64_t m = (uint64_t)n;
    seq_t result = (seq_t)1;

    /* Factor out 2 */
    if (m % 2ULL == 0ULL) {
        uint64_t exp = 0;
        while (m % 2ULL == 0ULL) {
            m /= 2ULL;
            exp++;
        }
        /* factor for 2^exp is always 1, so nothing changes */
    }

    /* Factor odd primes */
    for (uint64_t p = 3ULL; p * p <= m; p += 2ULL) {
        if (m % p == 0ULL) {
            uint64_t exp = 0;
            while (m % p == 0ULL) {
                m /= p;
                exp++;
            }
            seq_t factor;
            if (p == 5ULL) {
                factor = (seq_t)1;
            } else {
                uint64_t r = p % 20ULL;
                if (r == 1ULL || r == 3ULL || r == 7ULL || r == 9ULL) {
                    factor = (seq_t)(exp + 1ULL);
                } else if (r == 11ULL || r == 13ULL || r == 17ULL || r == 19ULL) {
                    if (exp % 2ULL == 0ULL) {
                        factor = (seq_t)1;
                    } else {
                        factor = (seq_t)0;
                    }
                } else {
                    /* All other primes > 5 are covered by the previous cases */
                    factor = (seq_t)1;
                }
            }
            result *= factor;
            if (result == (seq_t)0) {
                return result;
            }
        }
    }

    /* If remaining m > 1, it is a prime factor */
    if (m > 1ULL) {
        seq_t factor;
        if (m == 5ULL) {
            factor = (seq_t)1;
        } else {
            uint64_t r = m % 20ULL;
            if (r == 1ULL || r == 3ULL || r == 7ULL || r == 9ULL) {
                factor = (seq_t)(1ULL + 1ULL); /* exponent is 1 */
            } else if (r == 11ULL || r == 13ULL || r == 17ULL || r == 19ULL) {
                /* exponent 1 -> odd */
                factor = (seq_t)0;
            } else {
                factor = (seq_t)1;
            }
        }
        result *= factor;
    }

    return result;
}
