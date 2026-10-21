/*
 * A067559  Numbers n such that floor(Pi*n^2) is prime.
 * https://oeis.org/A067559
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers n for which floor(π·n²) is prime.  */
#include <stdint.h>
#include <stdlib.h>

static const uint64_t PI_SCALER = 3141592653589793238ULL;   /* π × 10¹⁸  */
static const uint64_t SCALE     = 1000000000000000000ULL;   /* 10¹⁸     */

/* Compute floor(π·n²) with integer arithmetic. */
static uint64_t floor_pi_sq(uint64_t n)
{
    __int128 prod = (__int128)PI_SCALER * n * n;
    return (uint64_t)(prod / SCALE);
}

/* Deterministic Miller–Rabin for 64‑bit integers.  Valid for all 0 < n < 2⁶⁴. */
static int is_prime_uint64(uint64_t n)
{
    if (n < 2)               return 0;
    if (n % 2 == 0)          return n == 2;
    if (n % 3 == 0)          return n == 3;
    if (n % 5 == 0)          return n == 5;
    if (n % 7 == 0)          return n == 7;
    if (n % 11 == 0)         return n == 11;
    if (n % 13 == 0)         return n == 13;
    if (n % 17 == 0)         return n == 17;

    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    /* Bases that are sufficient for all 64‑bit numbers. */
    const uint64_t bases[] = {2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL};
    for (size_t i = 0; i < sizeof(bases) / sizeof(bases[0]); ++i) {
        uint64_t a = bases[i];
        if (a % n == 0) continue;
        __uint128_t x = 1, base = a;
        uint64_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * base) % n;
            base = (base * base) % n;
            exp >>= 1;
        }
        if (x == 1 || x == n - 1) continue;
        int cont = 0;
        for (uint64_t r = 1; r < s; ++r) {
            x = (x * x) % n;
            if (x == n - 1) { cont = 1; break; }
        }
        if (!cont) return 0;
    }
    return 1;
}

/* Cache for discovered terms. */
static seq_t *cache = NULL;
static size_t cache_len   = 0;
static size_t cache_cap   = 0;
static idx_t  last_scanned = 1;

/* Ensure the cache has at least n terms. */
static void ensure_terms(idx_t n)
{
    if (cache_len >= (size_t)n) return;

    /* Grow cache capacity if necessary. */
    if (cache_cap == 0) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    } else if (cache_len == cache_cap) {
        cache_cap <<= 1;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }

    /* Scan numbers until we have n terms. */
    while (cache_len < (size_t)n) {
        uint64_t val = floor_pi_sq((uint64_t)last_scanned);
        if (is_prime_uint64(val)) {
            cache[cache_len++] = (seq_t)last_scanned;
        }
        ++last_scanned;
    }
}

/* Public sequence function.  a(1) is the first term. */
seq_t a(idx_t n)
{
    ensure_terms(n);
    return cache[n - 1];
}
