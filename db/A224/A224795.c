/*
 * A224795  Smallest prime q such that 2n+1 = 2p - q^3 for some odd prime p,
 *          or 0 if no such prime exists.
 * https://oeis.org/A224795
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest prime q such that 2n+1 = 2p - q^3 for some odd
 *            prime p, or 0 if no such prime exists
 * Range:     u8 n <= 62, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Needed headers */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Constants */
#define PRIME_LIMIT 200000

/* Static storage for prime list */
static bool primes_inited = false;
static uint32_t prime_arr[PRIME_LIMIT + 1];
static size_t prime_count = 0;

/* Simple sieve to fill prime_arr up to PRIME_LIMIT */
static void init_primes(void)
{
    if (primes_inited) return;
    bool mark[PRIME_LIMIT + 1] = {0};
    mark[0] = mark[1] = true;

    for (size_t i = 2; i * i <= PRIME_LIMIT; ++i)
        if (!mark[i])
            for (size_t j = i * i; j <= PRIME_LIMIT; j += i)
                mark[j] = true;

    size_t idx = 0;
    for (size_t i = 2; i <= PRIME_LIMIT; ++i)
        if (!mark[i])
            prime_arr[idx++] = (uint32_t)i;

    prime_count = idx;
    primes_inited = true;
}

/* Modular multiplication (a * b) % mod using 128-bit arithmetic */
static uint64_t mul_mod_uint64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)((__int128)a * b % mod);
}

/* Modular exponentiation (base^exp) % mod */
static uint64_t mod_pow_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1;
    uint64_t b = base % mod;
    while (exp)
    {
        if (exp & 1)
            result = mul_mod_uint64(result, b, mod);
        b = mul_mod_uint64(b, b, mod);
        exp >>= 1;
    }
    return result;
}

/* Deterministic Miller‑Rabin primality test for 64‑bit integers */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    /* Trial division with small primes */
    static const uint64_t small_primes[] = {
        2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL,
        23ULL, 29ULL, 31ULL, 37ULL, 0
    };
    for (size_t i = 0; small_primes[i]; ++i)
    {
        uint64_t p = small_primes[i];
        if (n % p == 0) return n == p;
    }

    /* Write n-1 as d * 2^s with d odd */
    uint64_t d = n - 1;
    uint32_t s = 0;
    while ((d & 1) == 0)
    {
        d >>= 1;
        ++s;
    }

    /* Deterministic bases for testing up to 2^64 */
    static const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL,
        9780504ULL, 1795265022ULL, 0
    };
    for (size_t i = 0; bases[i]; ++i)
    {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = mod_pow_uint64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (uint32_t r = 1; r < s; ++r)
        {
            x = mul_mod_uint64(x, x, n);
            if (x == n - 1)
            {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }
    return true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    init_primes();

    for (size_t i = 0; i < prime_count; ++i)
    {
        uint64_t q = prime_arr[i];
        if (q == 2) continue;   /* q must be odd prime */

        __int128 q3 = (__int128)q * q * q;          /* q^3 */
        __int128 p128 = (__int128)n + (q3 + 1) / 2;  /* p = n + (q^3+1)/2 */

        uint64_t p = (uint64_t)p128;             /* safe for our search limit */
        if (is_prime_uint64(p))
            return (seq_t)q;                    /* smallest such prime q found */
    }
    return (seq_t)0;  /* no suitable prime found */
}
