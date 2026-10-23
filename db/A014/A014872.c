/*
 * A014872  Numbers k that divide s(k), where s(1)=1, s(j)=21*s(j-1)+j.
 * https://oeis.org/A014872
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A014872: numbers k such that k divides s(k),
 * where s(1)=1 and s(j)=21*s(j-1)+j.  */

#include <stdlib.h>
#include <stdint.h>

/* dynamic cache of values a(1), a(2), ... */
static seq_t *cache      = NULL;   /* array of terms */
static size_t cache_len  = 0;      /* number of terms stored */
static size_t cache_cap  = 0;      /* current capacity of cache */
static idx_t  next_k     = 1;      /* next integer to test */

/* modular exponentiation using 128‑bit intermediate to avoid overflow */
static uint64_t powmod64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1ULL % mod;
    uint64_t b = base % mod;
    while (exp) {
        if (exp & 1ULL)
            result = (unsigned __int128)result * b % mod;
        b = (unsigned __int128)b * b % mod;
        exp >>= 1ULL;
    }
    return result;
}

/* ensure the cache has capacity 'needed' */
static void ensure_cap(size_t needed)
{
    if (cache_cap >= needed)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 4;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* generate terms until the cache contains at least 'n' terms */
static void extend_cache_until(idx_t n)
{
    while (cache_len < (size_t)n) {
        uint64_t k = (uint64_t)next_k;          /* current candidate */
        uint64_t mod = 400ULL * k;              /* 400*k  */
        uint64_t p   = powmod64(21ULL, k + 1ULL, mod);
        uint64_t rhs = 20ULL * k + 21ULL;       /* always < mod  */

        if (p == rhs) {                        /* k | s(k) condition   */
            ensure_cap(cache_len + 1);
            cache[cache_len++] = (seq_t)k;
        }
        ++next_k;                               /* advance to next candidate */
    }
}

/* public entry point: a(n) is the nth term of the sequence */
seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for n < 1 */
        return (seq_t)0;

    if (!cache) {        /* first call – initialise cache */
        cache_cap = 4;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_len = 0;
        next_k = 1;
    }

    extend_cache_until(n);
    return cache[n - 1];
}
