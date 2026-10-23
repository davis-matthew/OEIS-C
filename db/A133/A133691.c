/*
 * A133691  Expansion of (1 - (phi(-q) * phi(q^2))^2) / 4 in powers of q where
 *          phi() is a Ramanujan theta function.
 * https://oeis.org/A133691
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   a(n) is multiplicative with * a(1) = 1; * a(2) = -2, a(2^e) = -6
 *            for e > 1; * a(p^e) = (p^(e+1)-1)/(p-1) for odd primes p
 * Range:     i8 n <= 12, i16 n <= 210, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* No external headers are required for factorisation or exponentiation. */
#include <stdint.h>

/* Helper: compute base^exp for seq_t, where exp ≥ 0. */
static inline seq_t pow_seq_int(seq_t base, int exp)
{
    seq_t r = 1;
    for (int i = 0; i < exp; ++i)
        r *= base;
    return r;
}

/* Main function implementing a(n) for any n ≥ 1. */
seq_t a(idx_t n)
{
    if (n == 1)
        return 1;          /* a(1) = 1 */

    seq_t result = 1;
    uint64_t m = n;

    /* Handle factor 2 separately. */
    if ((m & 1ULL) == 0) {
        int e = 0;
        while ((m & 1ULL) == 0) {
            m >>= 1;
            ++e;
        }
        if (e == 1)
            result *= -2;
        else
            result *= -6;
    }

    /* Trial division for odd factors. */
    for (uint64_t p = 3; (unsigned __int128)p * p <= m; p += 2) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            seq_t pow = pow_seq_int((seq_t)p, e + 1);
            seq_t val = (pow - 1) / ((seq_t)(p - 1));
            result *= val;
        }
    }

    /* If a prime factor remains > 1, it is the last (and largest) prime. */
    if (m > 1) {
        seq_t pow = pow_seq_int((seq_t)m, 2);   /* e = 1 */
        seq_t val = (pow - 1) / ((seq_t)(m - 1));
        result *= val;
    }

    return result;
}
