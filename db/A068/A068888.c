/*
 * A068888  Squares with property that digits alternate in parity individually
 *          as well as in concatenation with previous terms.
 * https://oeis.org/A068888
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   is_alternating
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A068888: squares whose decimal digits alternate in parity,
   and whose leading digit parity alternates with the trailing digit
   parity of the previous term. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_alternating.h"

/* Forward declarations of helper functions. */
static bool is_alternating(seq_t x);
static uint8_t msd_parity(seq_t x);

seq_t a(idx_t n)
{
    /* ------------------------------------------------------------------ */
    /*  Static state: a memoised cache of accepted terms and the parity    */
    /*  of the last digit of the last accepted term.                       */
    /* ------------------------------------------------------------------ */
    static idx_t next_root = 1;            /* next integer whose square to test */
    static seq_t *cache      = NULL;       /* dynamic array of found terms     */
    static size_t  cache_cap = 0;          /* capacity of the cache            */
    static size_t  cache_len = 0;          /* number of terms stored           */
    static uint8_t prev_last_parity = 0;   /* last digit parity of previous   */

    /* initialise the cache once */
    if (cache == NULL) {
        cache_cap = 64;
        cache = (seq_t*)malloc(sizeof(seq_t) * cache_cap);
    }

    /* Generate more terms until we have at least n+1 elements. */
    while (cache_len <= n) {
        seq_t sq = (seq_t)next_root * (seq_t)next_root;

        if (is_alternating(sq)) {
            /* Check concatenation with previous term, if any. */
            if (cache_len > 0) {
                uint8_t first = msd_parity(sq);
                uint8_t last  = (uint8_t)((sq % 10) & 1);

                if (prev_last_parity == first) {
                    /* Boundary requirement fails – skip this square. */
                    goto next_root;
                }

                /* Accept this term. */
                if (cache_len == cache_cap) {
                    cache_cap *= 2;
                    cache = (seq_t*)realloc(cache, sizeof(seq_t) * cache_cap);
                }
                cache[cache_len++] = sq;
                prev_last_parity = last;
            } else {
                /* First term – no boundary to check. */
                if (cache_len == cache_cap) {
                    cache_cap *= 2;
                    cache = (seq_t*)realloc(cache, sizeof(seq_t) * cache_cap);
                }
                cache[cache_len++] = sq;
                prev_last_parity = (uint8_t)((sq % 10) & 1);
            }
        }

next_root:
        ++next_root;
    }

    return cache[n];
}

/* Return the parity (0 for even, 1 for odd) of the most significant
   decimal digit of x. */
static uint8_t msd_parity(seq_t x)
{
    while (x >= (seq_t)10) x /= 10;
    return (uint8_t)((x % 10) & 1);
}
