/*
 * A122860  Expansion of (1 - phi(-q)^3 / phi(-q^3)) / 6 in powers of q where
 *          phi() is a Ramanujan theta function.
 * https://oeis.org/A122860
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   a(n) is multiplicative. a(2^e) = (3*(-1)^e - 1) / 2 a(3^e) = 1
 *            a(p^e) = e + 1 if p ≡ 1 (mod 6) a(p^e) = (1 + (-1)^e)/2 if p ≡ 5
 *            (mod 6)
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)0) /* undefined for n = 0, return 0 for safety */
        return 0;
    uint64_t tmp = (uint64_t)n;
    seq_t result = 1;

    /* factor 2 */
    if ((tmp & 1ULL) == 0) {
        int e = 0;
        while ((tmp & 1ULL) == 0) {
            tmp >>= 1;
            ++e;
        }
        int sign = (e & 1) ? -1 : 1;
        seq_t val = ((seq_t)(3 * sign - 1)) / 2;
        result *= val;
        if (result == 0) return result;
    }

    /* factor 3 */
    if (tmp % 3ULL == 0) {
        for (; tmp % 3ULL == 0; tmp /= 3ULL) {
            /* a(3^e) = 1, nothing to do */
        }
    }

    /* factor remaining primes (p >= 5) */
    for (uint64_t p = 5; p * p <= tmp; p += 2) {
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            seq_t val;
            if (p % 6ULL == 1ULL) {
                val = (seq_t)(e + 1);
            } else { /* p % 6 == 5 */
                int sign = (e & 1) ? -1 : 1;
                val = ((seq_t)(1 + sign)) / 2;
            }
            result *= val;
            if (result == 0) return result;
        }
    }

    /* remaining prime factor (if any) */
    if (tmp > 1) {
        /* tmp is a prime > 3 (so 5 mod 6 or 1 mod 6) */
        uint64_t p = tmp; /* exponent is 1 */
        seq_t val;
        if (p % 6ULL == 1ULL) {
            val = (seq_t)2;     /* e = 1, so e + 1 = 2 */
        } else if (p % 6ULL == 5ULL) {
            val = 0;            /* (1 + (-1)^1)/2 = 0 */
        } else {
            val = 1;            /* shouldn't occur, safety */
        }
        result *= val;
    }

    return result;
}
