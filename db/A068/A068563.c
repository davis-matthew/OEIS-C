/*
 * A068563  Numbers k such that 2^k == 4^k (mod k).
 * https://oeis.org/A068563
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 1122, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A068563: numbers k such that 2^k ≡ 4^k (mod k).
   The function enumerates integers k starting at 1 and returns the
   n‑th solution (1‑indexed).  The test harness may call a() in any
   order; an internal cache holds all found solutions so that the
   algorithm works in O(n) time for the requested n. */

/* The implementation uses unsigned 128‑bit arithmetic for safe
 * modular multiplication when the modulus is as large as a 64‑bit
 * integer.  It is harmless for the small‑width seq_t builds. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---- helpers ------------------------------------------------------- */

/* modular exponentiation: computes (base^exp) mod mod.  base and mod
 * are < 2^64, exp < 2^64.  All intermediate multiplications are
 * performed in unsigned __int128 to avoid overflow. */
static uint64_t mod_pow_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    if (mod == 0) return 0;           /* undefined, but avoid division by 0 */
    uint64_t result = 1 % mod;
    uint64_t b = base % mod;
    while (exp)
    {
        if (exp & 1ULL)
            result = (uint64_t)((__uint128_t)result * b % mod);
        b = (uint64_t)((__uint128_t)b * b % mod);
        exp >>= 1ULL;
    }
    return result;
}

/* test whether a given k satisfies 2^k ≡ 4^k (mod k) */
static bool satisfies(idx_t k)
{
    uint64_t mod = (uint64_t)k;      /* work with a 64‑bit modulus */
    uint64_t a = mod_pow_uint64(2ULL, (uint64_t)k, mod);
    uint64_t b = mod_pow_uint64(4ULL, (uint64_t)k, mod);
    return a == b;
}

/* ---- cache for sequence values ------------------------------------- */

static seq_t *cache      = NULL;     /* array of found solutions   */
static idx_t  cap        = 0;        /* current capacity of cache */
static idx_t  count      = 0;        /* number of stored terms     */
static idx_t  last_k     = 0;        /* largest k already examined */

/* enlarge the cache so that it can hold at least 'need' elements */
static void grow_cache(idx_t need)
{
    if (need <= cap) return;
    idx_t new_cap = cap ? cap << 1 : 1;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
    if (!tmp) abort();  /* out of memory */
    cache = tmp;
    cap   = new_cap;
}

/* ---- required function -------------------------------------------- */

/* Public entry point for the sequence.  The first term (n==1) is 1. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for 0, return 0 as safe sentinel */

    /* Generate terms until we have at least n of them. */
    while (count < n)
    {
        ++last_k;                            /* next integer to test   */
        if (satisfies(last_k))
        {
            grow_cache(count + 1);
            cache[count++] = (seq_t)last_k;   /* store the solution     */
        }
    }
    return cache[n - 1];                      /* 1‑based indexing       */
}
