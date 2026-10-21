/*
 * A061596  Product of digits + 1 is prime, product of digits - 1 is prime,
 *          sum of digits + 1 is prime and sum of digits - 1 is prime.
 * https://oeis.org/A061596
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 846, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mul_mod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A061596
   a(n) is the nth integer (n≥0) such that:
     * P  = product of its decimal digits
     * S  = sum of its decimal digits
     * P−1, P+1, S−1, and S+1 are all prime
   The sequence starts 4,6,22,114,123,… (offset 0). */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mul_mod64.h"

/* ---------- prime test for 64‑bit integers ---------- */

static uint64_t pow_mod64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t res = 1 % mod;
    while (exp) {
        if (exp & 1ULL) res = mul_mod64(res, base, mod);
        base = mul_mod64(base, base, mod);
        exp >>= 1ULL;
    }
    return res;
}
static bool is_prime_64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};
    for (size_t i = 0; i < sizeof(small)/sizeof(small[0]); ++i) {
        if (n == small[i]) return true;
        if (n % small[i] == 0ULL) return false;
    }
    uint64_t d = n-1, s = 0;
    while ((d & 1ULL) == 0ULL) { d >>= 1ULL; ++s; }
    static const uint64_t bases[] =
        {2ULL,325ULL,9375ULL,28178ULL,450775ULL,9780504ULL,1795265022ULL};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0ULL) continue;
        uint64_t x = pow_mod64(a, d, n);
        if (x == 1ULL || x == n-1) continue;
        bool comp = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = mul_mod64(x, x, n);
            if (x == n-1) { comp = false; break; }
        }
        if (comp) return false;
    }
    return true;
}

/* ---------- test whether n satisfies the conditions ---------- */
static bool qualifies(uint64_t n)
{
    uint64_t prod = 1;
    uint64_t sum  = 0;
    bool has_even = false;

    if (n == 0) return false;         /* digits cannot contain 0 */
    while (n > 0) {
        unsigned d = n % 10;
        n /= 10;
        if (d == 0) return false;     /* zero digit → product 0 */

        /* keep product within 64‑bit bounds */
        if (prod > (UINT64_MAX - d) / d) return false;
        prod *= d;

        sum += d;
        if ((d & 1) == 0) has_even = true;
    }

    if (!has_even) return false;      /* sum must be even */
    if (sum & 1ULL) return false;     /* sum odd → S−1 or S+1 even >2 */

    if (!is_prime_64(prod + 1)) return false;
    if (!is_prime_64(prod - 1)) return false;
    if (!is_prime_64(sum  + 1)) return false;
    if (!is_prime_64(sum  - 1)) return false;

    return true;
}

/* ---------- cache of found terms ---------- */
static seq_t *cache     = NULL;
static size_t cache_len = 0;   /* number of terms stored */
static size_t cache_cap = 0;   /* allocated capacity */

static void ensure_cache_cap(size_t need)
{
    if (cache_cap >= need) return;
    size_t new_cap = cache_cap ? cache_cap : 8;
    while (new_cap < need) new_cap <<= 1U;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* generate terms until at least n+1 terms are cached */
static void generate_up_to(idx_t n)
{
    static uint64_t last_checked = 0;
    while (cache_len <= (size_t)n) {
        ++last_checked;
        if (qualifies(last_checked)) {
            ensure_cache_cap(cache_len + 1);
            cache[cache_len++] = (seq_t)last_checked;
        }
    }
}

/* ---------- public sequence function ---------- */
seq_t a(idx_t n)
{
    generate_up_to(n);
    return cache[(size_t)n];
}
