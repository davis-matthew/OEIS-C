/*
 * A106404  Number of even semiprimes dividing n.
 * https://oeis.org/A106404
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n is even) ? ω(n/2) : 0 where ω(m) is the number of
 *            distinct prime divisors of m
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* small helper: count distinct prime factors of u, assumes u>0 */
static seq_t count_distinct_prime_factors(uint64_t u) {
    seq_t cnt = 0;
    if (u == 1) return 0;
    if (u & 1ULL) {
        /* odd part, reserve for later */
    } else {
        cnt++;                     /* factor 2 */
        while ((u & 1ULL) == 0) u >>= 1;
    }
    for (uint64_t p = 3; p <= u / p; p += 2) {
        if (u % p == 0) {
            cnt++;
            while (u % p == 0) u /= p;
        }
    }
    if (u > 1) cnt++;              /* remaining prime > sqrt(original) */
    return cnt;
}

seq_t a(idx_t n) {
    if ((n & 1ULL) == 1) return 0;   /* odd n → no even semiprime divisors */
    uint64_t half = n >> 1;          /* n/2, fits in uint64_t because n is uint64_t */
    return count_distinct_prime_factors(half);
}
