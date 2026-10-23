/*
 * A161153  Positive integers that are coprime to their number of digits in
 *          binary representation.
 * https://oeis.org/A161153
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 151, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   bitlen_seq_t, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A161153: Positive integers that are coprime to their binary digit count.
   a(n) = n-th positive integer k such that gcd(k, ⌊log₂(k)⌋+1) = 1.
   The values are generated lazily on demand and cached for reuse. */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/bitlen_seq_t.h"

/* static cache for already computed terms */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* ensure the cache has room for at least 'need' elements */
static void grow_cache(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < need)
        new_cap *= 2;
    seq_t *ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = ptr;
    cache_cap = new_cap;
}

/* Return the n-th term of the sequence (1‑based index) */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)   /* undefined; return 0 for safety */
        return (seq_t)0;

    /* Extend the cache until we have at least n terms */
    if (cache_len < (size_t)n) {
        /* start checking from the next integer after the last cached term */
        idx_t next = cache_len ? (idx_t)(cache[cache_len - 1] + (seq_t)1) : 1;

        while (cache_len < (size_t)n) {
            /* value to test */
            seq_t v = (seq_t)next;

            /* binary length of v */
            unsigned bits = bitlen_seq_t(v);

            /* add v to cache if coprime to its binary length */
            if (gcd(v, (seq_t)bits) == (seq_t)1) {
                grow_cache(cache_len + 1);
                cache[cache_len++] = v;
            }
            next++;
        }
    }
    return cache[n - 1];
}
