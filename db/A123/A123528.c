/*
 * A123528  Numerator of average of prime factors of n.
 * https://oeis.org/A123528
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = numerator of sopfr(n) / bigomega(n)
 * Range:     u8 n <= 256, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* undefined below offset, return 0 */

    uint64_t m = (uint64_t)n;     /* copy for factorisation */

    seq_t sum = 0;                /* sum of prime factors (with multiplicity) */
    seq_t cnt = 0;                /* total number of prime factors (with multiplicity) */

    /* factor out powers of 2 first */
    while ((m & 1ULL) == 0) {
        sum += 2;
        cnt++;
        m >>= 1; /* divide by 2 */
    }

    /* factor odd primes */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        while (m % p == 0) {
            sum += p;
            cnt++;
            m /= p;
        }
    }

    /* if something remains, it is a prime > sqrt(original n) */
    if (m > 1) {
        sum += m;
        cnt++;
    }

    /* reduce the fraction sum / cnt to lowest terms and return numerator */
    seq_t g = gcd(sum, cnt);
    return sum / g;
}
