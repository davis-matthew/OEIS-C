/*
 * A053864  A second-order generalization of the Mobius function of n.
 * https://oeis.org/A053864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 0 if any exponent in the prime factorization of n exceeds
 *            2; * a(n) = 1 if there is at least one exponent equal to 1 (and
 *            none > 2); * a(n) = (-1)^omega(n) when all exponents are exactly
 *            2, * where omega(n) is the number of distinct prime factors
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1; a(1) = 1. */
    if (n == 1)
        return 1;

    uint64_t m = n;          /* use unsigned 64‑bit integer for factoring */
    int has_less = 0;        /* flag for exponent 1 */
    int omega = 0;           /* count of distinct prime factors */

    /* Factor the power of 2 separately. */
    int exp = 0;
    while ((m & 1ULL) == 0) {
        m >>= 1;
        exp++;
    }
    if (exp > 0) {
        omega++;
        if (exp > 2)
            return 0;
        if (exp == 1)
            has_less = 1;
    }

    /* Factor odd primes by trial division. */
    for (uint64_t d = 3; d * d <= m; d += 2) {
        if (m % d != 0)
            continue;

        exp = 0;
        while (m % d == 0) {
            m /= d;
            exp++;
        }

        omega++;
        if (exp > 2)
            return 0;
        if (exp == 1)
            has_less = 1;
    }

    /* Any remaining m > 1 is a prime factor with exponent 1. */
    if (m > 1) {
        omega++;
        has_less = 1;
    }

    /* Apply the rules. */
    if (has_less)
        return 1;                 /* at least one exponent = 1, all ≤ 2 */
    return (omega % 2 == 0) ? 1 : -1;   /* all exponents = 2 */
}
