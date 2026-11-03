/*
 * A143697  Least square k^2 such that n^2-k^2 = p*q with p and q odd primes
 *          and p<q for n>= 4.
 * https://oeis.org/A143697
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the smallest k² < n² such that n² – k² = p·q * where p
 *            and q are distinct odd primes (p < q). If no such k exists *
 *            a(n) is returned as 0 (this never happens for n ≥ 4)
 * Range:     u8 n <= 42, u16 n <= 5130, u32 n <= 60000, u64 n <= 60000,
 *            u128 n <= 60000, big any n
 * Validated: standard, pass, 87 terms through n = 9491 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A143697: the least square k^2 with n^2 - k^2 = p q for odd primes p < q
 * (n >= 4). For k = 1, 2, ... of the other parity from n (so n^2 - k^2 is odd),
 * the smallest prime factor p of n^2 - k^2 is found by trial division and
 * q = (n^2 - k^2)/p tested with the exact 64-bit primality test. (The original
 * capped its prime table at 20000.) */

#include <stdint.h>
#include "../utils/is_prime_u64.h"

static uint64_t least_factor(uint64_t m)    /* m odd, m > 1 */
{
    for (uint64_t d = 3; d * d <= m; d += 2)
        if (m % d == 0) return d;
    return m;
}

seq_t a(idx_t n)
{
    if (n < 4) return (seq_t)0;
    const uint64_t N = (uint64_t)n;
    for (uint64_t k = 1; k < N; ++k) {
        if ((N & 1) == (k & 1)) continue;
        uint64_t m = N * N - k * k;
        uint64_t p = least_factor(m);
        if (p == m) continue;                  /* m prime */
        uint64_t q = m / p;
        if (q != p && is_prime_u64(q)) return (seq_t)(k * k);
    }
    return (seq_t)0;
}
