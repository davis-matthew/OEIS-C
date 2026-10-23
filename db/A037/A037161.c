/*
 * A037161  Well-order the rational numbers; take numerators.
 * https://oeis.org/A037161
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, nice, frac
 * Formula:   a(n) = numerator (with sign) of the n-th rational in the *
 *            well-order of the rationals by increasing |p|+q, negative
 *            numerators * listed before positives. a(0) = 0
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute φ(m), the Euler totient of m.  m > 0. */
static unsigned long long phi_uint64(unsigned long long m)
{
    unsigned long long result = m;
    unsigned long long temp = m;
    for (unsigned long long p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            while (temp % p == 0)
                temp /= p;
            result -= result / p;
        }
    }
    if (temp > 1)
        result -= result / temp;
    return result;
}

/* Greatest common divisor for unsigned long long. */
static unsigned long long gcd_ull(unsigned long long a, unsigned long long b)
{
    while (b != 0) {
        unsigned long long t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* target is the (n-1)-th element among the enumerated fractions
     * (excluding the leading 0/1). */
    const unsigned long long target = (unsigned long long)n - 1ULL;

    unsigned long long cumulative = 0ULL;  /* sum of 2φ(t) up to current s-1 */
    unsigned long long s = 2ULL;          /* current sum |p|+q */

    /* Find the group (sum s) that contains the required term. */
    while (1) {
        unsigned long long phi_s = phi_uint64(s);
        unsigned long long group_size = 2ULL * phi_s;   /* negative + positive */

        /* If target lies within this group, stop. */
        if (target < cumulative + group_size)
            break;

        cumulative += group_size;
        ++s;
    }

    unsigned long long phi_s = phi_uint64(s);
    unsigned long long k = target - cumulative; /* 0‑based index inside the group */

    if (k < phi_s) {
        /* Negative part: p runs from -(s-1) down to -1 (i.e. |p| from s-1 to 1). */
        unsigned long long count = 0;
        for (long long p = (long long)s - 1; p >= 1; --p) {
            unsigned long long q = s - (unsigned long long)p;
            if (gcd_ull((unsigned long long)p, q) == 1ULL) {
                if (count == k)
                    return -(seq_t)p;
                ++count;
            }
        }
    } else {
        /* Positive part: p runs from 1 up to s-1. */
        unsigned long long posk = k - phi_s;
        unsigned long long count = 0;
        for (unsigned long long p = 1; p <= s - 1; ++p) {
            unsigned long long q = s - p;
            if (gcd_ull(p, q) == 1ULL) {
                if (count == posk)
                    return (seq_t)p;
                ++count;
            }
        }
    }

    /* Should never be reached. */
    return (seq_t)0;
}
