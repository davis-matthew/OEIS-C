/*
 * A191763  Integers that cannot be partitioned into a sum of an odd square,
 *          an even square and a triangular number.
 * https://oeis.org/A191763
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 38, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   all_factors_congruent_1_mod4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th triangular number T(m) where all prime factors of
   2m+1 are congruent to 1 mod 4.  The sequence starts with
   a(1)=3, a(2)=21, a(3)=36, ...   (offset 1). */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/all_factors_congruent_1_mod4.h"

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static uint64_t last_m = 1;
static bool initialized = false;

/* Ensure static structures are allocated. */
static void seq_a_init(void)
{
    if (initialized) return;
    cache_cap = 128;
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    cache_len = 0;
    initialized = true;
}

seq_t a(idx_t n)
{
    seq_a_init();

    /* Return cached value if already computed. */
    if (n <= cache_len) return cache[n - 1];

    /* Find further terms until we have at least n. */
    while (cache_len < n) {
        last_m++;                         /* consider next candidate m */
        uint64_t x = 2 * last_m + 1;       /* 2m+1 */
        if (!all_factors_congruent_1_mod4(x)) continue;

        /* compute T(m) = m*(m+1)/2 */
        seq_t val = (seq_t)last_m * ((seq_t)last_m + 1) / 2;

        /* grow cache if necessary */
        if (cache_len >= cache_cap) {
            cache_cap *= 2;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }
        cache[cache_len++] = val;
    }

    return cache[n - 1];
}
