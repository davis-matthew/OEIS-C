/*
 * A160777  Numbers that have an "e" in the middle of their names in Spanish.
 * https://oeis.org/A160777
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, word
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   has_middle_e, number_to_spanish
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A160777: Numbers whose Spanish name has an “e” in the middle.
   The function searches the natural numbers, keeps the found terms
   in a memoized cache, and returns the n‑th term (1‑based). */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/has_middle_e.h"
#include "../utils/number_to_spanish.h"

/* ---------- helper to build a Spanish name ---------- */

/* ---------- test if a number has an 'e' in the centre of its Spanish name ---------- */

/* ---------- memoised cache for the sequence ---------- */

static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap  = 0;
static uint64_t next_to_check = 1;

static void ensure_cache_capacity(size_t need)
{
    if (cache_size >= need) return;
    if (cache_cap < need) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < need) new_cap = need;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* ---------- main sequence function ---------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* offset is 1 */
    while (cache_size < (size_t)n) {
        seq_t val = (seq_t)next_to_check;
        if (has_middle_e(next_to_check)) {
            ensure_cache_capacity(cache_size + 1);
            cache[cache_size++] = val;
        }
        next_to_check++;
    }
    return cache[n - 1];
}
