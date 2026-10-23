/*
 * A167341  Totally multiplicative sequence with a(p) = p*(p-3) = p^2-3p for
 *          prime p.
 * https://oeis.org/A167341
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = product over prime powers p^e || n of (p*(p-3))^e. a(1) =
 *            1, a(3k) = 0 for k >= 1
 * Range:     i8 n <= 12, i16 n <= 190, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t */

seq_t a(idx_t n) {
    /* Handle offset 1.  n >= 1 is assumed by the contract. */
    if (n % 3ULL == 0)          /* includes n = 3 */
        return 0;

    seq_t result = 1;
    uint64_t m = (uint64_t)n;
    int exp;

    /* factor 2 separately (may have high exponent) */
    exp = 0;
    while ((m & 1ULL) == 0) {   /* m divisible by 2 */
        m >>= 1;
        exp++;
    }
    if (exp > 0) {
        seq_t factor = (seq_t)2 * ((seq_t)2 - 3);   /* 2*(-1) = -2 */
        for (int i = 0; i < exp; ++i)
            result *= factor;
    }

    /* trial division for odd primes */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            exp = 0;
            while (m % p == 0) {
                m /= p;
                exp++;
            }
            /* p == 3 cannot happen here because n % 3 == 0 was tested before.
               The check is kept for safety. */
            if (p == 3)
                return 0;
            seq_t factor = (seq_t)p * ((seq_t)p - 3);
            for (int i = 0; i < exp; ++i)
                result *= factor;
        }
    }

    /* remaining prime factor, if any */
    if (m > 1) {
        if (m == 3)
            return 0;        /* defensive, though caught above */
        seq_t factor = (seq_t)m * ((seq_t)m - 3);
        result *= factor;    /* exponent is 1 */
    }

    return result;
}
