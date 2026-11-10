/*
 * A060398  Values of k associated with A060397.
 * https://oeis.org/A060398
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = least k >= 0 with p | k^2+k+2n+1, where p = A060397(n) is
 *            the smallest prime dividing any k^2+k+2n+1; found by search over
 *            primes p and residues k
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 121 terms through n = 7249 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int A060398_is_prime(uint64_t n)
{
    if (n < 2) return 0;
    if (n % 2ULL == 0ULL) return n == 2ULL;
    for (uint64_t i = 3ULL; i * i <= n; i += 2ULL)
        if (n % i == 0ULL) return 0;
    return 1;
}

seq_t a(idx_t n)
{
    uint64_t nn = (uint64_t)n;
    for (uint64_t p = 3ULL; ; p += 2ULL)
    {
        if (!A060398_is_prime(p)) continue;
        uint64_t c = (2ULL * (nn % p) + 1ULL) % p;
        for (uint64_t k = 0; k < p; k++)
        {
            if ((k * k + k + c) % p == 0ULL)
                return (seq_t)k;
        }
    }
}
