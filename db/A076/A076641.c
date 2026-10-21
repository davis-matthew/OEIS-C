/*
 * A076641  Integers read backwards, but with repetitions omitted.
 * https://oeis.org/A076641
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = n‑th element of the sequence of integers obtained by *
 *            reversing the decimal representation of successive non‑negative
 *            * integers, but omitting any value that has already appeared
 * Range:     u8 n <= 92, u16 n <= 9006, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* static storage for memoisation */
static seq_t *cache = NULL;
static size_t cache_len = 0, cache_cap = 0;

/* bit‑array of seen reversed numbers (so far) */
static unsigned char *seen = NULL;
static size_t seen_cap = 0;

/* next natural number to test */
static uint64_t next_nat = 0;

/* reverse a uint64_t in decimal, ignoring leading zeros */
static uint64_t reverse_uint64(uint64_t x)
{
    uint64_t rev = 0;
    do {
        rev = rev * 10 + (x % 10);
        x   = x / 10;
    } while (x);
    return rev;
}

/* grow the seen array to accommodate value v */
static void ensure_seen_cap(uint64_t v)
{
    if (v < seen_cap) return;
    size_t new_cap = seen_cap ? seen_cap * 2 : 1;
    while (new_cap <= v) new_cap <<= 1;
    seen = (unsigned char*)realloc(seen, new_cap);
    memset(seen + seen_cap, 0, new_cap - seen_cap);
    seen_cap = new_cap;
}

/* grow the cache array to at least 'need' entries */
static void ensure_cache_cap(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 1;
    while (new_cap < need) new_cap <<= 1;
    cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    memset((char*)cache + cache_len * sizeof(seq_t),
           0, (new_cap - cache_len) * sizeof(seq_t));
    cache_cap = new_cap;
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* if already computed, return immediately */
    if (cache_len > (size_t)n)
        return cache[(size_t)n];

    /* generate terms until we reach index n */
    for (; cache_len <= (size_t)n; ++next_nat) {
        uint64_t rev = reverse_uint64(next_nat);
        ensure_seen_cap(rev);
        if (!seen[rev]) {
            seen[rev] = 1;
            ensure_cache_cap(cache_len + 1);
            cache[cache_len++] = rev;
        }
    }
    return cache[(size_t)n];
}
