/*
 * A076561  a(1)=2; a(n>1)= greatest prime divisor of {a(n-1) + n}.
 * https://oeis.org/A076561
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 231, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A076561:
   a(1) = 2; a(n>1) = greatest prime divisor of (a(n-1) + n) */
#include <stdlib.h>
#include <stdint.h>

/* Cached terms (1‑based).  Static allocation is rebuilt lazily. */
static seq_t *cache   = NULL;
static idx_t cache_len = 0;

/* Greatest prime factor of a 64‑bit integer.
   Works for values that fit into uint64_t; if the value is larger,
   the highest prime factor is returned as the value itself. */
static seq_t lpf_uint64(uint64_t v)
{
    uint64_t best = 1;

    if (v % 2ULL == 0ULL) {
        best = 2ULL;
        while (v % 2ULL == 0ULL) v /= 2ULL;
    }
    for (uint64_t p = 3ULL; p * p <= v; p += 2ULL) {
        if (v % p == 0ULL) {
            best = p;
            while (v % p == 0ULL) v /= p;
        }
    }
    if (v > 1ULL) best = v;
    return (seq_t)best;
}

/* Make sure the cache contains values up to index n. */
static void ensure_cache(idx_t n)
{
    if (n <= cache_len) return;

    size_t needed = (size_t)n + 1;              /* 1‑based indexing */
    seq_t *tmp = (seq_t*)realloc(cache, needed * sizeof(seq_t));
    if (!tmp) abort();                          /* allocation failure */
    cache = tmp;

    for (idx_t i = cache_len + 1; i <= n; ++i) {
        if (i == 1U) {
            cache[1] = (seq_t)2;                 /* a(1) = 2 */
        } else {
            /* a(n) = lpf( a(n-1) + n ) */
            /* use unsigned 128‑bit to avoid overflow when seq_t is 128‑bit */
            unsigned __int128 sum128 = (unsigned __int128)cache[i-1] + (unsigned __int128)i;
            uint64_t sum64   = (uint64_t)sum128;   /* value is expected to fit in 64 bits */
            cache[i] = lpf_uint64(sum64);
        }
    }
    cache_len = n;
}

/* Public entry point required by the build. */
seq_t a(idx_t n)
{
    if (n < 1U) return (seq_t)0;
    ensure_cache(n);
    return cache[n];
}
