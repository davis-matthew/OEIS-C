/*
 * A166597  Let p = largest prime <= n, with p(0)=p(1)=0, and let q = smallest
 *          prime > n; then a(n) = q-p.
 * https://oeis.org/A166597
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = q - p where p = largest prime ≤ n (p(0)=p(1)=0), and q =
 *            smallest prime > n
 * Range:     u8 n <= 9220, u16 n <= 9220, u32 n <= 9220, u64 n <= 9220,
 *            u128 n <= 9220, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* modular multiplication using 128‑bit intermediate to avoid overflow */
static uint64_t mulmod_u64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)((__uint128_t)a * b % mod);
}

/* modular exponentiation (a^e mod mod) */
static uint64_t modpow_u64(uint64_t a, uint64_t e, uint64_t mod)
{
    uint64_t res = 1 % mod;
    a %= mod;
    while (e)
    {
        if (e & 1)
            res = mulmod_u64(res, a, mod);
        a = mulmod_u64(a, a, mod);
        e >>= 1;
    }
    return res;
}

/* Deterministic Miller–Rabin for 64‑bit integers */
static bool is_prime_u64(uint64_t n)
{
    if (n < 2) return false;

    /* small primes */
    static const uint64_t small_primes[] =
        {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};
    for (size_t i = 0; i < sizeof(small_primes) / sizeof(small_primes[0]); ++i)
    {
        if (n == small_primes[i]) return true;
        if (n % small_primes[i] == 0) return false;
    }

    /* write n-1 as d*2^s with d odd */
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1ULL) == 0) {
        d >>= 1;
        ++s;
    }

    /* bases sufficient for 64‑bit numbers */
    static const uint64_t bases[] =
        {2ULL,325ULL,9375ULL,28178ULL,450775ULL,9780504ULL,1795265022ULL};
    for (size_t i = 0; i < sizeof(bases) / sizeof(bases[0]); ++i)
    {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = modpow_u64(a, d, n);
        if (x == 1 || x == n-1) continue;
        bool composite = true;
        for (int r = 1; r < s; ++r) {
            x = mulmod_u64(x, x, n);
            if (x == n-1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* largest prime ≤ n (p(0)=p(1)=0) */
static uint64_t prev_prime_u64(uint64_t n)
{
    if (n < 2) return 0;
    if (n == 2) return 2;
    if ((n & 1ULL) == 0) --n;          // make n odd
    for (uint64_t x = n; x >= 2; x -= 2) {
        if (is_prime_u64(x)) return x;
        if (x <= 3) break;             // avoid underflow
    }
    return 0;
}

/* smallest prime > n */
static uint64_t next_prime_u64(uint64_t n)
{
    if (n < 2) return 2;
    uint64_t x = n + 1;
    if ((x & 1ULL) == 0 && x != 2) ++x;   // start from the next odd
    for (;;) {
        if (is_prime_u64(x)) return x;
        if (x >= UINT64_MAX - 2) break;   // safety guard
        x += 2;
    }
    return 0;  /* unreachable for valid inputs */
}

/* required sequence function */
seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)2;     /* for n = 0 or 1, p = 0, q = 2 */

    uint64_t p = prev_prime_u64((uint64_t)n);   /* largest prime ≤ n */
    uint64_t q = next_prime_u64((uint64_t)n);   /* smallest prime > n */

    return (seq_t)(q - p);
}
