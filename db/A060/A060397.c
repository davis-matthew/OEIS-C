/*
 * A060397  Smallest prime that divides k^2 + k + 2n + 1 for k = 0,1,2,....
 * https://oeis.org/A060397
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 3 if n ≡ 0 or 1 (mod 3) a(n) = minimal prime p ≠ 3 such
 *            that ∃k : k² + k + 2n + 1 ≡ 0 (mod p), i.e. the discriminant of
 *            x² + x + (2n+1) is a quadratic residue modulo p
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static uint64_t pow_mod_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t res = 1 % mod;
    base %= mod;
    while (exp)
    {
        if (exp & 1ULL)
            res = (unsigned __int128)res * base % mod;
        base = (unsigned __int128)base * base % mod;
        exp >>= 1ULL;
    }
    return res;
}

static int is_prime_uint64(uint64_t n)
{
    if (n < 2) return 0;
    if (n % 2ULL == 0ULL) return n == 2ULL;
    if (n % 3ULL == 0ULL) return n == 3ULL;
    for (uint64_t i = 5ULL; i * i <= n; i += 6ULL)
    {
        if (n % i == 0ULL) return 0;
        if (n % (i + 2ULL) == 0ULL) return 0;
    }
    return 1;
}

seq_t a(idx_t n)
{
    /* Special cases: n mod 3 == 0 or 1 */
    if ((n % 3ULL) == 0ULL || (n % 3ULL) == 1ULL)
        return (seq_t)3;

    /* n mod 3 == 2: search primes >3 in increasing order */
    /* Starting from 5, skip even numbers */
    for (uint64_t p = 5ULL; ; p += 2ULL)
    {
        if (!is_prime_uint64(p))
            continue;

        /* c = (2*n + 1) mod p */
        uint64_t c = (2ULL * (uint64_t)n + 1ULL) % p;
        /* discriminant D = 1 - 4*c mod p */
        uint64_t D = (1ULL + p - (4ULL * c) % p) % p;
        if (D == 0ULL)
            return (seq_t)p; /* D==0 -> perfect square, root exists */

        /* Legendre symbol: D^((p-1)/2) mod p == 1 or -1 */
        uint64_t leg = pow_mod_uint64(D, (p - 1ULL) / 2ULL, p);
        if (leg == 1ULL) /* D is a quadratic residue */
            return (seq_t)p;
        /* else continue searching */
    }
}
