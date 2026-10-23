/*
 * A192330  Minimum number of endpoints of a tree so that there exists a
 *          zero-entropy map defined on it having a period n orbit.
 * https://oeis.org/A192330
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - Σ_{i=2..k} Π_{j=i..k} s_j, * where n = Π_{j=1..k} s_j
 *            is the prime factorisation of n * (s_j non‑decreasing)
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)n;           /* a(1)=1, a(0) not used */

    uint64_t factors[64];                  /* at most 64 prime factors for 64‑bit n */
    size_t nf = 0;
    uint64_t tmp = n;

    /* factor out 2 */
    while ((tmp & 1ULL) == 0ULL) {
        factors[nf++] = 2ULL;
        tmp >>= 1;
    }

    /* factor odd primes */
    for (uint64_t p = 3; p <= tmp / p; p += 2) {
        while (tmp % p == 0ULL) {
            factors[nf++] = p;
            tmp /= p;
        }
    }
    if (tmp > 1ULL) {                      /* remaining prime factor */
        factors[nf++] = tmp;
    }

    seq_t prod = 1;                         /* suffix product */
    seq_t sum  = 0;                         /* Σ of suffix products for i≥2 */

    for (int i = (int)nf - 1; i >= 0; --i) {
        prod *= (seq_t)factors[i];
        if (i >= 1) sum += prod;
    }

    return (seq_t)n - sum;
}
