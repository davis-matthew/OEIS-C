/*
 * A057302  Numbers n such that n | 5^n + 4^n + 1.
 * https://oeis.org/A057302
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 31, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A057302: Numbers n such that n | 5^n + 4^n + 1
 *
 *  OEIS implementation: iterate n upward, test divisibility.
 *  The test uses modular exponentiation with 128‑bit safety.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* --- modular exponentiation for up to 64‑bit modulus -------------------- */
static inline uint64_t mod_pow_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    base %= mod;
    while (exp) {
        if (exp & 1ULL)
            result = (uint64_t)((__uint128_t)result * base % mod);
        base = (uint64_t)((__uint128_t)base * base % mod);
        exp >>= 1ULL;
    }
    return result;
}

/* --- helper : does n divide 5^n + 4^n + 1? ---------------------------- */
static bool satisfies(idx_t n)
{
    uint64_t mod  = (uint64_t)n;                     /* n > 0 guaranteed   */
    uint64_t s5   = mod_pow_uint64(5ULL, mod, mod);
    uint64_t s4   = mod_pow_uint64(4ULL, mod, mod);
    uint64_t sum  = (s5 + s4 + 1ULL) % mod;
    return sum == 0ULL;
}

/* --- dynamic cache of terms ------------------------------------------- */
static seq_t *cache        = NULL;
static size_t cache_cap    = 0;
static size_t cache_len    = 0;
static idx_t  next_candidate = 1;

/* grow the cache to at least 'need' entries */
static void grow_cache(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* fill cache up to 'upto' terms (1‑based index) */
static void ensure_terms(idx_t upto)
{
    while (cache_len < (size_t)upto) {
        if (satisfies(next_candidate))
            cache[cache_len++] = (seq_t)next_candidate;
        ++next_candidate;
    }
}

/* ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)                /* offset is 1 – n==0 is undefined */
        return (seq_t)0;
    if (!cache)                /* first call */
        cache = (seq_t *)calloc(16, sizeof(seq_t));
    grow_cache((size_t)n);
    ensure_terms(n);
    return cache[n - 1];
}
