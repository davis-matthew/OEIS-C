/*
 * A063127  Composite numbers which in base 2 contain their largest proper
 *          factor as a substring.
 * https://oeis.org/A063127
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) of OEIS A063127:
   composite numbers whose binary representation contains
   their largest proper divisor as a substring.  The function
   builds the sequence lazily, caching the values so that calls
   can come in any order. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/* Helper: smallest prime factor of a 64‑bit integer (trial division). */
static uint64_t _spf_uint64(uint64_t n)
{
    if (n < 2) return n;
    if ((n & 1ULL) == 0) return 2ULL;
    for (uint64_t d = 3ULL; d * d <= n; d += 2ULL)
        if (n % d == 0) return d;
    return n;                          /* prime */
}

/* ------------------------------------------------------------------ */
/* Helper: number of bits needed to represent x (x > 0). */
static inline unsigned bitlen_uint64(uint64_t x)
{
    return 64 - __builtin_clzll(x);
}

/* ------------------------------------------------------------------ */
/* Check whether binary representation of sub occurs in that of big. */
static bool contains_sub_uint64(uint64_t big, uint64_t sub)
{
    if (sub == 0) return true;           /* zero is trivially contained */
    unsigned bl = bitlen_uint64(big);
    unsigned sl = bitlen_uint64(sub);
    if (sl > bl) return false;
    uint64_t mask = (1ULL << sl) - 1ULL;
    for (unsigned shift = 0; shift <= bl - sl; ++shift) {
        if (((big >> shift) & mask) == sub) return true;
    }
    return false;
}

/* ------------------------------------------------------------------ */
/* Cache infrastructure – stores terms as uint64_t to avoid issues with
   non‑trivial seq_t types.  Values are converted to seq_t on return. */
static uint64_t *cache_uint64 = NULL;
static size_t cache_len  = 0;
static size_t cache_cap  = 0;
static uint64_t next_candidate = 4ULL;   /* smallest composite > 2 */

/* Ensure the cache can hold at least 'need' elements. */
static void cache_grow_if_needed(size_t need)
{
    if (cache_cap >= need) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need) new_cap *= 2;
    uint64_t *new_ptr = (uint64_t*)realloc(cache_uint64, new_cap * sizeof(uint64_t));
    if (!new_ptr) { /* allocation failure – abort */ abort(); }
    cache_uint64 = new_ptr;
    cache_cap = new_cap;
}

/* Append a new term to the cache. */
static void cache_append(uint64_t val)
{
    cache_grow_if_needed(cache_len + 1);
    cache_uint64[cache_len++] = val;
}

/* ------------------------------------------------------------------ */
/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) {                 /* offset is 1; return 0 for safety */
        return 0;
    }

    while (cache_len < (size_t)n) {
        uint64_t cand = next_candidate++;
        /* Skip primes. */
        uint64_t spf = _spf_uint64(cand);
        if (spf == cand) continue;          /* cand is prime */

        /* Largest proper divisor: cand / spf. */
        uint64_t lpd = cand / spf;

        if (contains_sub_uint64(cand, lpd)) {
            cache_append(cand);
        }
    }
    return (seq_t)cache_uint64[n - 1];
}
