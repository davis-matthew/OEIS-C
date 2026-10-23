/*
 * A138745  Expansion of eta(q) * eta(q^3) * eta(q^4)^3 / (eta(q^2)^2 *
 *          eta(q^12)) in powers of q.
 * https://oeis.org/A138745
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -b(n) where b is the multiplicative function defined by:
 *            b(2^e) = -1 (for e>0) b(3^e) = 2 - (-1)^e (e>0) b(p^e) = e+1 if
 *            p ≡ 1 or 5 (mod 12) b(p^e) = (1 + (-1)^e)/2 if p ≡ 7 or 11 (mod
 *            12) The sequence starts with a(0)=1
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/build_primes.h"

seq_t a(idx_t n)
{
    /* offset: a(0) = 1 */
    if (n == 0) return 1;

    /* static storage for primes up to 100 000 (covers n ≤ 10¹⁰) */
    static idx_t *primes = NULL;
    static size_t primes_cnt = 0;
    if (primes == NULL) {
        /* 100 000 is large enough for all test cases up to ~10¹⁰. */
        build_primes(100000, &primes, &primes_cnt);
    }

    idx_t m = n;
    seq_t prod = 1;          /* product of b(p^e) values */

    /* trial division by precomputed primes */
    for (size_t i = 0; i < primes_cnt && (idx_t)primes[i] * (idx_t)primes[i] <= m; ++i) {
        idx_t p = primes[i];
        if (m % p != 0) continue;

        int exp = 0;
        do { m /= p; ++exp; } while (m % p == 0);

        seq_t v;
        if (p == 2) {
            /* b(2^e) = -1 for any e>0 */
            v = -1;
        } else if (p == 3) {
            /* b(3^e) = 2 - (-1)^e */
            v = (exp % 2 == 0) ? 1 : 3;
        } else {
            idx_t mod = p % 12;
            if (mod == 1 || mod == 5) {
                /* b(p^e) = e+1 */
                v = (seq_t)(exp + 1);
            } else { /* mod == 7 or 11 */
                v = (exp % 2 == 0) ? 1 : 0;
            }
        }

        prod *= v;
        if (prod == (seq_t)0) return 0;   /* zero propagates */
    }

    /* any remaining prime factor > sqrt(n) */
    if (m > 1) {            /* exponent is 1 */
        idx_t p = m;
        seq_t v;
        if (p == 2) {
            v = -1;
        } else if (p == 3) {
            v = 3;          /* e = 1 is odd */
        } else {
            idx_t mod = p % 12;
            if (mod == 1 || mod == 5) {
                v = 2;      /* e + 1 = 2 */
            } else {        /* mod == 7 or 11 */
                v = 0;
            }
        }
        prod *= v;
        if (prod == (seq_t)0) return 0;
    }

    /* a(n) = -b(n) */
    return -prod;
}
