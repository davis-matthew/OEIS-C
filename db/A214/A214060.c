/*
 * A214060  Least m>0 such that gcd(2*n-1+m, n-m) > 1.
 * https://oeis.org/A214060
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if n is odd; otherwise a(n) = min over prime divisors p
 *            of (3n−1) of (n%p==0 ? p : n%p)
 * Range:     u8 n <= 257, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* External types used by the build */
extern seq_t a(idx_t n);

seq_t a(idx_t n)
{
    /* For odd n, m = 1 always works: 2n and n-1 are both even. */
    if (n & 1ULL) {
        return (seq_t)1;
    }

    /* n is even: factor D = 3n - 1 */
    unsigned long long N = (unsigned long long)n;   /* n fits in 64 bits for the tests */
    unsigned long long D = 3ULL * N - 1ULL;          /* D ≤ 3n-1 */

    unsigned long long best = N;          /* a(n) ≤ n */

    /* Trial division to find all distinct prime factors of D */
    unsigned long long d = D;
    for (unsigned long long p = 2; p * p <= d; ++p) {
        if (d % p == 0ULL) {
            /* Process prime factor p */
            unsigned long long cand = (N % p == 0ULL) ? p : (N % p);
            if (cand < best) best = cand;

            /* Skip all multiples of p */
            while (d % p == 0ULL) {
                d /= p;
            }
        }
    }
    /* Remaining factor d, if any, is prime > 1 */
    if (d > 1ULL) {
        unsigned long long cand = (N % d == 0ULL) ? d : (N % d);
        if (cand < best) best = cand;
    }

    return (seq_t)best;
}
