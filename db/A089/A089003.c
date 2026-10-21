/*
 * A089003  Number of non-congruent solutions to x^2 - 2y^2 == 1 (mod n).
 * https://oeis.org/A089003
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, nonn, easy
 * Formula:   a(n) = ∏_{p^e∥n} a(p^e) where a(2^e) = 2^e (e≤2) , 2^(e+1) (e>2)
 *            a(p^e) = (p−1)·p^(e−1) if p≡±1 (mod 8) a(p^e) = (p+1)·p^(e−1) if
 *            p≡±3 (mod 8) for odd primes and a(1)=1
 * Range:     u8 n <= 95, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;
    seq_t result = 1;
    idx_t tmp = n;
    /* factor 2 separately */
    idx_t e2 = 0;
    while ((tmp & 1ULL) == 0) {        /* tmp % 2 == 0 */
        tmp >>= 1;
        ++e2;
    }
    if (e2) {
        if (e2 <= 2) {
            result *= (seq_t)1ULL << e2;          /* 2^e2 */
        } else {
            result *= (seq_t)1ULL << (e2 + 1);    /* 2^(e2+1) */
        }
    }
    /* odd factors */
    for (idx_t p = 3; p <= (idx_t) (tmp ? (idx_t) (tmp / p) : 0) + 1; p += 2) {
        if (tmp % p == 0) {
            idx_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            /* compute p^(e-1) */
            seq_t ppow = 1;
            if (e > 1) {
                for (idx_t k = 1; k < e; ++k)
                    ppow *= (seq_t)p;
            }
            seq_t factor;
            if (p % 8 == 1 || p % 8 == 7) {
                factor = (seq_t)(p - 1) * ppow;
            } else {        /* p ≡ 3 or 5 (mod 8) */
                factor = (seq_t)(p + 1) * ppow;
            }
            result *= factor;
        }
    }
    /* remaining prime factor */
    if (tmp > 1) {
        seq_t p = (seq_t)tmp;    /* tmp is prime, e=1 */
        seq_t factor;
        if (p % 8 == 1 || p % 8 == 7) {
            factor = p - 1;      /* p^(e-1)=1 */
        } else {
            factor = p + 1;
        }
        result *= factor;
    }
    return result;
}
