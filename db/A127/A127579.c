/*
 * A127579  Primes of the form 64n+63.
 * https://oeis.org/A127579
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 194, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A127579: primes of the form 64n + 63, n >= 1. */

/*  The routine generates the primes on demand using a deterministic
    Miller–Rabin test for 64‑bit integers.  A dynamic array stores
    the primes found so far so that later calls can reuse them without
    recomputation.  This approach works for seq_t of any unsigned type
    (including arbitrary‑precision) as long as the value fits into
    seq_t. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Deterministic Miller–Rabin for 64‑bit values.
   Bases are sufficient for all unsigned 64‑bit integers. */
static uint64_t mod_mul(uint64_t a, uint64_t b, uint64_t mod)
{
#if defined(__SIZEOF_INT128__)
    return (unsigned __int128)a * b % mod;
#else
    uint64_t res = 0;
    a %= mod;
    while (b) {
        if (b & 1)
            res = (res + a) % mod;
        a = (a << 1) % mod;
        b >>= 1;
    }
    return res;
#endif
}

static uint64_t mod_pow(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1;
    while (exp) {
        if (exp & 1)
            result = mod_mul(result, base, mod);
        base = mod_mul(base, base, mod);
        exp >>= 1;
    }
    return result;
}

static bool is_prime_uint64(uint64_t n)
{
    if (n < 2)
        return false;
    /* small primes */
    static const uint64_t small_primes[] = {
        2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL,
        19ULL, 23ULL, 29ULL, 31ULL, 37ULL, 41ULL, 43ULL,
        47ULL, 53ULL, 59ULL, 61ULL, 67ULL, 71ULL, 73ULL,
        79ULL, 83ULL, 89ULL, 97ULL
    };
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        if (n == small_primes[i])
            return true;
        if (n % small_primes[i] == 0)
            return false;
    }
    /* write n-1 as d*2^s */
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }
    /* witnesses for deterministic test up to 2^64 */
    static const uint64_t witnesses[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL,
        450775ULL, 9780504ULL, 1795265022ULL
    };
    for (size_t i = 0; i < sizeof(witnesses)/sizeof(witnesses[0]); ++i) {
        uint64_t a = witnesses[i] % n;
        if (a == 0)
            continue;
        uint64_t x = mod_pow(a, d, n);
        if (x == 1 || x == n - 1)
            continue;
        bool composite = true;
        for (int r = 1; r < s; ++r) {
            x = mod_mul(x, x, n);
            if (x == n - 1) {
                composite = false;
                break;
            }
        }
        if (composite)
            return false;
    }
    return true;
}

/* Dynamic array of primes found. */
static seq_t *prime_cache   = NULL;
static size_t prime_cache_cap = 0;
static size_t prime_cache_len = 0;
static uint64_t next_candidate_k = 1;   /* k for 64k+63 */

/* Grow the prime cache when needed. */
static void ensure_prime_cache_cap(void)
{
    if (prime_cache_len < prime_cache_cap)
        return;
    size_t new_cap = prime_cache_cap == 0 ? 64 : prime_cache_cap * 2;
    seq_t *tmp = (seq_t *)realloc(prime_cache, new_cap * sizeof(seq_t));
    if (!tmp)
        return; /* allocation failure; subsequent calls will fail gracefully */
    prime_cache = tmp;
    prime_cache_cap = new_cap;
}

/* Append a new prime to the cache. */
static void append_prime(uint64_t val)
{
    ensure_prime_cache_cap();
    prime_cache[prime_cache_len++] = (seq_t)val;
}

/* Produces the n-th prime of the required form. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    while (prime_cache_len < (size_t)n) {
        uint64_t cand = 64ULL * next_candidate_k + 63ULL;
        ++next_candidate_k;
        if (is_prime_uint64(cand))
            append_prime(cand);
    }
    return prime_cache[(size_t)n - 1];
}
