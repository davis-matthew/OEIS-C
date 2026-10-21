/*
 * A236475  Numbers k such that k^3 + k - 1 is prime.
 * https://oeis.org/A236475
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th integer k such that k^3 + k – 1 is prime
 * Range:     u8 n <= 58, u16 n <= 6699, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------------
   Tiny deterministic Miller–Rabin for 64‑bit integers.
   Correct for all 0 ≤ n < 2^64.
   ------------------------------------------------------------------ */
static inline uint64_t mul_mod64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)((__int128)a * b % mod);
}

static inline uint64_t pow_mod64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    uint64_t cur    = base % mod;
    while (exp) {
        if (exp & 1ULL)
            result = mul_mod64(result, cur, mod);
        cur = mul_mod64(cur, cur, mod);
        exp >>= 1;
    }
    return result;
}

static inline bool is_prime_u64(uint64_t n)
{
    if (n < 2)
        return false;
    /* small primes */
    static const uint64_t small[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};
    for (size_t i = 0; i < sizeof small / sizeof *small; ++i) {
        if (n == small[i])
            return true;
        if (n % small[i] == 0)
            return false;
    }

    /* Miller–Rabin with bases sufficient for 64‑bit */
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1ULL) == 0) {
        d >>= 1;
        ++s;
    }

    static const uint64_t bases[] = {2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL};
    for (size_t i = 0; i < sizeof bases / sizeof *bases; ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0)
            continue;
        uint64_t x = pow_mod64(a, d, n);
        if (x == 1 || x == n - 1)
            continue;
        bool composite = true;
        for (int r = 1; r < s; ++r) {
            x = mul_mod64(x, x, n);
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

/* ------------------------------------------------------------------
   Cache for the sequence terms.  The cache holds values of type
   seq_t (which may be any unsigned integer type).  The last tested k
   is stored to allow searching from the previous position.
   ------------------------------------------------------------------ */
static seq_t *seq_cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t last_k_checked = 0; /* last integer k that was examined */

/* Ensure the cache can hold at least 'needed' elements. */
static void ensure_cache_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* Return the nth term (1‑based) of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0)               /* Offset is 1; undefined for n==0 */
        return (seq_t)0;

    while (cache_len < n) {
        ++last_k_checked;
        /* Compute k^3 + k – 1 in 128‑bit arithmetic to avoid overflow. */
        uint64_t k = (uint64_t)last_k_checked;
        __uint128_t val = (__uint128_t)k * k * k;
        val += k;
        val -= 1;

        if (val <= UINT64_MAX) {
            if (is_prime_u64((uint64_t)val)) {
                /* Append k to cache. */
                ensure_cache_capacity(cache_len + 1);
                seq_cache[cache_len++] = (seq_t)k;
            }
        } else {
            /* If we exceed 64‑bit range we stop; the required n
               will not be reached with such large k in the given limits. */
            break;
        }
    }
    return seq_cache[n - 1];
}
