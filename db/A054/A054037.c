/*
 * A054037  Numbers k such that k^2 contains exactly 9 different digits.
 * https://oeis.org/A054037
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 665, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   digit_frequencies, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation searches for integers k whose square contains
 * exactly 9 distinct decimal digits.  It caches the results: when
 * a(n) is requested, it continues searching from the last candidate
 * until the n‑th number is found, then returns that stored value.
 * The function works for any seq_t that supports the provided
 * multiplication (seq_mul) and digit frequency (digit_frequencies)
 * utilities.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/seq_mul.h"
#include "../utils/digit_frequencies.h"

static seq_t *cache = NULL;           /* array of found numbers */
static size_t cache_cap = 0;          /* allocated capacity */
static size_t cache_len = 0;          /* number of stored elements */
static idx_t next_candidate = 1;      /* next integer to test */

/* Ensure the cache has room for at least needed elements. */
static void ensure_cache_cap(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap : 8;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = new_ptr;
    cache_cap = new_cap;
}

/* Return the nth term of sequence A054037 (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0)  /* offset is 1, return 0 for safety */
        return 0;

    /* Search until we have at least n terms stored. */
    while (cache_len < (size_t)n) {
        /* Test next_candidate */
        seq_t val = (seq_t)next_candidate;
        seq_t sq  = seq_mul(val, val);

        unsigned int freq[10] = {0};
        digit_frequencies(sq, freq);

        unsigned int distinct = 0;
        for (int d = 0; d < 10; ++d)
            if (freq[d] > 0)
                ++distinct;

        if (distinct == 9) {
            /* Store the found value */
            ensure_cache_cap(cache_len + 1);
            cache[cache_len++] = val;
        }
        ++next_candidate;
    }

    return cache[n - 1];
}
