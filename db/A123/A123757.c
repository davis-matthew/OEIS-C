/*
 * A123757  a(0)=1. a(n) = sum of the earlier terms which are divisible by
 *          (the number of 1's in the binary representation of n).
 * https://oeis.org/A123757
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A123757:
   a(0) = 1
   a(n) = sum of all earlier terms a(k) (k < n) that are divisible
   by popcount(n), the number of 1’s in the binary representation
   of n.  popcount(n) is never 0 for n > 0, so only divisors 1…64
   are needed for the sums.  */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Portable pop‑count for 64‑bit integers.  */
static inline unsigned int popcount_u64(uint64_t x)
{
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_popcountll(x);
#else
    unsigned int c = 0;
    while (x) { c++; x &= x - 1; }
    return c;
#endif
}

/* ------------------------------------------------------------------
   seq_t is the user’s integer type (uint8_t … __int128 … big)
   idx_t is the index type (uint64_t or similar).

   The function keeps a static cache of all already computed terms
   and an auxiliary array 'sums[d]' holding the sum of all terms
   divisible by d (for d = 1 … 64).  Because popcount(n) never
   exceeds 64, a single array of size 65 suffices.
   ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* static state --------------------------------------------------- */
    static bool initialized = false;
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_len = 0;
    static seq_t sums[65];             /* sums[d] = sum of earlier terms divisible by d */

    if (!initialized) {
        for (int i = 0; i < 65; ++i)
            sums[i] = (seq_t)0;
        initialized = true;
    }

    /* make sure the cache is large enough for indices 0 … n inclusive */
    if ((size_t)n + 1 > cache_cap) {
        size_t newcap = cache_cap ? cache_cap * 2ULL : 64ULL;
        while (newcap < (size_t)n + 1)
            newcap *= 2ULL;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) abort();          /* allocator failure – terminate */
        cache = tmp;
        cache_cap = newcap;
    }

    /* compute any missing terms up to n */
    for (idx_t k = cache_len; k <= n; ++k) {
        seq_t val;
        if (k == 0) {
            val = (seq_t)1; /* a(0) = 1 */
        } else {
            unsigned int pc = popcount_u64((uint64_t)k);
            /* sums[pc] contains the sum of all earlier terms divisible by pc */
            val = sums[pc];
        }
        cache[k] = val;

        /* update the sums array for the new term 'val' */
        for (int d = 1; d <= 64; ++d) {
            /* test divibility; seq_t promotion to integral type */
            if (val % (seq_t)d == (seq_t)0)
                sums[d] = sums[d] + val;
        }
    }
    cache_len = (idx_t)(n + 1);

    return cache[n];
}
