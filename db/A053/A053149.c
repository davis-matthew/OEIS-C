/*
 * A053149  Smallest cube divisible by n.
 * https://oeis.org/A053149
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = the smallest cube divisible by n. For each prime power
 *            p^e in n, use exponent e' = e + ((3 - e) mod 3) = ceil(e/3) * 3,
 *            and multiply p^e' over all primes
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1625, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>


/* computes p^exp as seq_t   */
static inline seq_t pow_uint64(unsigned long long base, unsigned exp)
{
    seq_t result = 1;
    seq_t b = (seq_t)base;
    while (exp) {
        if (exp & 1) result *= b;
        exp >>= 1;
        if (exp) b *= b;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    unsigned long long temp = (unsigned long long)n;
    seq_t res = 1;

    /* factor 2 */
    unsigned e2 = 0;
    while ((temp & 1ULL) == 0) {
        e2++;
        temp >>= 1;
    }
    if (e2) {
        unsigned rem = e2 % 3;
        unsigned exp = e2 + (rem ? 3 - rem : 0);
        res *= pow_uint64(2ULL, exp);
    }

    /* odd primes */
    for (unsigned long long p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            unsigned e = 0;
            while (temp % p == 0) {
                temp /= p;
                e++;
            }
            unsigned rem = e % 3;
            unsigned exp = e + (rem ? 3 - rem : 0);
            res *= pow_uint64(p, exp);
        }
    }

    if (temp > 1) {          /* remaining prime factor (exponent 1) */
        res *= pow_uint64(temp, 3);
    }

    return res;
}
