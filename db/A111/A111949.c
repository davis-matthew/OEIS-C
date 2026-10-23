/*
 * A111949  Expansion of eta(q) * eta(q^2) * eta(q^10) * eta(q^20) / (eta(q^4)
 *          * eta(q^5)) in powers of q.
 * https://oeis.org/A111949
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   a(n) is multiplicative with a(p^e) = (-1)^e if p = 2 1 if p = 5
 *            (1 + (-1)^e)/2 if p ≡ 11,13,17,19 (mod 20) e + 1 if p ≡ 1,9 (mod
 *            20) (e + 1)(-1)^e if p ≡ 3,7 (mod 20)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Helper to compute the value of a single prime power factor */
static seq_t prime_power_value(uint64_t p, uint64_t e)
{
    if (p == 2) {
        return (e & 1ULL) ? (seq_t)-1 : (seq_t)1;
    }
    if (p == 5) {
        return (seq_t)1;
    }
    uint64_t mod20 = p % 20ULL;
    switch (mod20) {
        case 11: case 13: case 17: case 19:
            return (e & 1ULL) ? (seq_t)0 : (seq_t)1;
        case 1: case 9:
            return (seq_t)(e + 1);
        case 3: case 7:
            return ((e & 1ULL) ? (seq_t)-1 : (seq_t)1) * (seq_t)(e + 1);
        default:
            return (seq_t)1;          /* should never happen for odd primes */
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) {            /* undefined for n < 1; return 0 */
        return (seq_t)0;
    }
    if (n == 1) {
        return (seq_t)1;
    }

    seq_t result = (seq_t)1;
    uint64_t temp = (uint64_t)n;

    /* factor 2 */
    uint64_t e2 = 0;
    while ((temp & 1ULL) == 0ULL) {
        temp >>= 1;
        ++e2;
    }
    if (e2) {
        seq_t val = prime_power_value(2ULL, e2);
        result *= val;
        if (result == (seq_t)0) return result;
    }

    /* odd primes */
    for (uint64_t p = 3ULL; p <= temp / p; p += 2ULL) {
        if (temp % p == 0ULL) {
            uint64_t e = 0;
            while (temp % p == 0ULL) {
                temp /= p;
                ++e;
            }
            seq_t val = prime_power_value(p, e);
            result *= val;
            if (result == (seq_t)0) return result;
        }
    }

    /* remaining prime factor > sqrt(original n) */
    if (temp > 1ULL) {
        seq_t val = prime_power_value(temp, 1ULL);
        result *= val;
    }

    return result;
}
