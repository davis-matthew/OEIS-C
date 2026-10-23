/*
 * A141859  Primes congruent to 12 mod 13.
 * https://oeis.org/A141859
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 538, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Method: iterate over numbers 12 (mod 13) and test for primality with
   deterministic Miller–Rabin (is_prime64).  Memoize found primes so
   that repeated calls are cheap.  The sequence is valid for any
   seq_t that fits the produced primes (up to at least 64‑bit). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* ---------- Miller–Rabin for 64‑bit integers ---------- */
static inline uint64_t modmul64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)(((__uint128_t)a * b) % mod);
}

static inline uint64_t modpow64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1;
    uint64_t b = base % mod;
    while (exp) {
        if (exp & 1)
            result = modmul64(result, b, mod);
        b = modmul64(b, b, mod);
        exp >>= 1;
    }
    return result;
}

static bool is_prime64(uint64_t n)
{
    if (n < 2)
        return false;

    /* small prime trial division */
    static const uint64_t small[] =
        { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37 };
    for (size_t i = 0; i < sizeof(small) / sizeof(small[0]); ++i) {
        uint64_t p = small[i];
        if (n == p) return true;
        if (n % p == 0) return false;
    }

    /* Miller–Rabin deterministic bases for 64‑bit */
    uint64_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }

    static const uint64_t bases[] =
        { 2, 325, 9375, 28178, 450775, 9780504, 1795265022 };
    for (size_t i = 0; i < sizeof(bases) / sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = modpow64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool cont = false;
        for (unsigned r = 1; r < s; ++r) {
            x = modmul64(x, x, n);
            if (x == n - 1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* ---------- realloc helper for C++ ---------- */
static inline seq_t *realloc_seq(void *ptr, size_t size)
{
#ifdef __cplusplus
    return static_cast<seq_t *>(realloc(ptr, size));
#else
    return (seq_t *)realloc(ptr, size);
#endif
}

/* ---------- cache of primes ≡ 12 (mod 13) ---------- */
static seq_t *prime_cache     = NULL;   /* stored primes */
static size_t cache_len       = 0;       /* number of stored primes */
static size_t cache_cap       = 0;       /* allocated capacity   */
static uint64_t next_candidate = 12;     /* next 12‑mod‑13 candidate */

/* ensure at least 'needed' slots in cache */
static void ensure_capacity(size_t needed)
{
    if (cache_cap < needed) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < needed) new_cap = needed;
        seq_t *tmp = realloc_seq(prime_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* out of memory */
        prime_cache = tmp;
        cache_cap   = new_cap;
    }
}

/* ---------- public sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* out of range, undefined */

    if (prime_cache == NULL)
        ensure_capacity(16);          /* initial allocation */

    /* extend cache until we have n primes */
    while (cache_len < n) {
        /* find the next prime of form 13k+12 */
        while (true) {
            if (is_prime64(next_candidate)) {
                ensure_capacity(cache_len + 1);
                prime_cache[cache_len++] = (seq_t)next_candidate;
                /* advance to the next candidate after storing */
                next_candidate += 13;
                break;
            }
            next_candidate += 13;      /* try next 12‑mod‑13 number */
        }
    }
    return prime_cache[n - 1];
}
