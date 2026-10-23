/*
 * A161511  Number of 1...0 pairs in the binary representation of 2n.
 * https://oeis.org/A161511
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(2n)=a(n)+A000120(n); a(2n+1)=a(n)+1; a(0)=0
 * Range:     u8 n <= 8192, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* static cache for memoisation */
static seq_t *cache = NULL;
static bool *seen  = NULL;
static idx_t cache_cap = 0;

/* Ensure we can store indices up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (n < cache_cap) return;
    idx_t new_cap = cache_cap ? cache_cap : 128;
    while (new_cap <= n) new_cap <<= 1;
    seq_t *tmpc = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    bool  *tmpb = (bool  *)realloc(seen , new_cap * sizeof(bool ));
    /* initialise newly allocated slots as unseen */
    for (idx_t i = cache_cap; i < new_cap; ++i) tmpb[i] = false;
    cache = tmpc;
    seen  = tmpb;
    cache_cap = new_cap;
}

/* Recursive computation with memoisation. */
static seq_t rec_a(idx_t n)
{
    if (n == 0) return 0;
    if (seen[n]) return cache[n];

    idx_t half = n >> 1;
    seq_t val = rec_a(half);
    if ((n & 1ULL) == 0) {                 /* n even */
        val += (seq_t)__builtin_popcountll((unsigned long long)half);
    } else {                               /* n odd  */
        val += (seq_t)1;
    }

    ensure_cache(n);
    cache[n] = val;
    seen[n]  = true;
    return val;
}

/* Public interface required by the build. */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return rec_a(n);
}
