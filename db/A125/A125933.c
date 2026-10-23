/*
 * A125933  Sprague-Grundy values for octal game .7307.
 * https://oeis.org/A125933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   mex_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sprague–Grundy values for octal game .7307.
   a(n) = mex({ Grundy of all legal moves from a heap of size n }).
   In the octal notation the three bits of each digit describe the
   allowed results after removing k tokens:
        bit 0 (value 1) – the position becomes empty (no heaps),
        bit 1 (value 2) – leave the remaining tokens as a single heap,
        bit 2 (value 4) – split the remaining tokens into two heaps. */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/mex_of.h"

/* Octal digits for removing 1…4 tokens.  The index 0 is unused. */
static const unsigned char OCTAL_D[5] = {0, 7, 3, 0, 7};

static seq_t *cache = NULL;        /* cached Grundy values */
static size_t  cache_cap = 0;       /* current allocation size   */
static size_t  cache_len = 0;       /* number of filled entries  */

static const size_t MEX_LIMIT = 256; /* sufficient for the game */

seq_t a(idx_t n)
{
    if (n == 0) return 0u;  /* a(0) is unused, but keep it defined */

    /* initialise the cache with the base case a(0)=0 */
    if (cache_len == 0) {
        cache      = (seq_t *)calloc(1, sizeof(seq_t));  /* cache[0] = 0 */
        cache_cap  = 1;
        cache_len  = 1;
    }

    /* compute all values up to n on demand */
    while (cache_len <= (size_t)n) {
        /* grow the allocation when necessary */
        if (cache_cap <= (size_t)n) {
            size_t new_cap = cache_cap * 2;
            if (new_cap <= (size_t)n) new_cap = (size_t)n + 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache = tmp;
            cache_cap = new_cap;
        }

        size_t i = cache_len;         /* heap size to evaluate */
        bool reach[MEX_LIMIT];
        memset(reach, 0, sizeof(reach));

        for (int k = 1; k <= 4; ++k) {
            if (i < (size_t)k) continue;            /* cannot remove k tokens */

            unsigned char d = OCTAL_D[k];
            if (d == 0) continue;                   /* no move for this k */

            size_t rem = i - k;                     /* tokens left after removal */

            /* empty move (bit 0) */
            if (d & 1) reach[0] = true;

            /* single‑heap move (bit 1) */
            if (d & 2) {
                seq_t g1 = cache[rem];
                if ((size_t)g1 < MEX_LIMIT) reach[(size_t)g1] = true;
            }

            /* split move (bit 2) */
            if (d & 4) {
                for (size_t a = 1; a < rem; ++a) {
                    seq_t g = cache[a] ^ cache[rem - a];
                    if ((size_t)g < MEX_LIMIT) reach[(size_t)g] = true;
                }
            }
        }

        cache[i] = mex_of(reach, MEX_LIMIT);
        ++cache_len;
    }

    return cache[(size_t)n];
}
