/*
 * A096786  Numbers n such that both n and n+1 are composite numbers that sum
 *          up to a Pythagorean prime (i.e., of the form 4k+1).
 * https://oeis.org/A096786
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th integer m such that * m and m+1 are composite, *
 *            and 2m+1 is a prime ≡ 1 (mod 4)
 * Range:     u8 n <= 33, u16 n <= 5255, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mul128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Necessary headers for both C and C++ */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mul128.h"

static uint64_t mod_pow(uint64_t a, uint64_t e, uint64_t mod)
{
    __uint128_t result = 1;
    __uint128_t base   = a % mod;
    while (e) {
        if (e & 1) result = (result * base) % mod;
        base = (base * base) % mod;
        e >>= 1;
    }
    return (uint64_t)result;
}

static bool is_prime(uint64_t n)
{
    if (n < 2) return false;
    /* small primes */
    static const uint64_t small[] =
        {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};
    for (size_t i = 0; i < sizeof small / sizeof small[0]; ++i) {
        uint64_t p = small[i];
        if (n % p == 0) return n == p;
    }
    /* Miller–Rabin deterministic for 64‑bit */
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }
    static const uint64_t bases[] =
        {2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL};
    for (size_t i = 0; i < sizeof bases / sizeof bases[0]; ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = mod_pow(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool comp = true;
        for (int r = 1; r < s; ++r) {
            x = mul128(x, x, n);
            if (x == n - 1) { comp = false; break; }
        }
        if (comp) return false;
    }
    return true;
}

/* ---------- Cache for already computed terms ---------- */
static seq_t   *cache   = NULL;   /* dynamically grown array of terms   */
static size_t   cache_cap = 0;    /* array capacity                     */
static size_t   cache_len = 0;    /* number of terms stored             */
static uint64_t next_candidate = 2; /* next integer to test */

static void grow_cache(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap < need) new_cap <<= 1;
    seq_t *new_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_arr) exit(1);        /* fatal: out of memory            */
    cache = new_arr;
    cache_cap = new_cap;
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                  /* undefined, but harmless     */
    if (n <= cache_len) return cache[n-1]; /* memoised result             */

    grow_cache(n);
    while (cache_len < n) {
        /* Find next integer satisfying the conditions */
        for (;;) {
            if (!is_prime(next_candidate) &&
                !is_prime(next_candidate + 1)) {
                uint64_t s = 2 * next_candidate + 1;
                if ((s % 4 == 1) && is_prime(s))
                    break;
            }
            ++next_candidate;
        }
        /* Store the term */
        cache[cache_len++] = (seq_t)next_candidate;
        ++next_candidate;                   /* move past the term found       */
    }
    return cache[n-1];
}
