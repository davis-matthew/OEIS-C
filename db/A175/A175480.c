/*
 * A175480  a(1)=1. For n >= 1, if a(n) = 1, then append to the sequence the
 *          digits of binary n+1 (most significant digits first and least
 *          significant digits last). If a(n) = 0, then append to the sequence
 *          the digits of binary n+1 in reverse order (least significant
 *          digits first and most significant digits l...
 * https://oeis.org/A175480
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Construction:
   a(1)=1.
   For n ≥ 1, let d = a(n).
   If d = 1, append to the sequence the binary digits of n+1
   from most‑significant to least‑significant.
   If d = 0, append the binary digits of n+1 in reverse order. */

#include <stddef.h>
#include <stdbool.h>
/* helpers for memory management of seq_t arrays */
#include "../utils/seq_alloc.h"   /* seq_t* seq_alloc(size_t cap);   */
#include "../utils/seq_realloc.h" /* seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap); */

seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1; n>=1 in the actual problem */
        return (seq_t)0;

    /* static cached value of the sequence */
    static seq_t *cache      = NULL;   /* array of bits 0/1 */
    static size_t cache_len  = 0;      /* number of terms currently stored */
    static size_t cache_cap  = 0;      /* allocated capacity */
    static idx_t  processed  = 0;      /* largest n that has been processed */

    /* initial allocation and first term a(1)=1 */
    if (cache == NULL) {
        cache_cap  = 16;
        cache      = seq_alloc(cache_cap);
        cache[0]   = (seq_t)1;
        cache_len  = 1;
    }

    /* generate terms until we have at least n terms in cache */
    while (processed < n) {
        idx_t cur = processed + 1;                /* current n to process */
        seq_t dir = cache[cur - 1];                /* a(cur) */
        idx_t num = cur + 1;                       /* binary(n+1) */

        /* determine bit length of num */
        idx_t tmp   = num;
        idx_t bits  = 0;
        while (tmp) { bits++; tmp >>= 1; }
        if (bits == 0) bits = 1;   /* safety though num ≥ 2 */

        /* append bits of num in the correct order */
        if (dir == (seq_t)1) {     /* most‑significant to least‑significant */
            for (idx_t k = bits; k > 0; --k) {
                seq_t bit = (seq_t)((num >> (k - 1)) & 1);
                if (cache_len == cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
                    cache = seq_realloc(cache, cache_cap, new_cap);
                    cache_cap = new_cap;
                }
                cache[cache_len++] = bit;
            }
        } else {                   /* least‑significant to most‑significant */
            for (idx_t k = 0; k < bits; ++k) {
                seq_t bit = (seq_t)((num >> k) & 1);
                if (cache_len == cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap << 1 : 16;
                    cache = seq_realloc(cache, cache_cap, new_cap);
                    cache_cap = new_cap;
                }
                cache[cache_len++] = bit;
            }
        }

        processed = cur;          /* this n is now processed */
    }

    return cache[n - 1];
}
