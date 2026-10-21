/*
 * A084088  Numbers k such that k == 2 (mod 3) and the exponent of the highest
 *          power of 2 dividing k is even.
 * https://oeis.org/A084088
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   v2_even
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A084088: numbers k such that k ≡ 2 (mod 3) and the exponent of the
   highest power of 2 dividing k is even. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/v2_even.h"

/* --- Cached values --------------------------------------------- */
static seq_t *cache = NULL;      /* array of found numbers */
static idx_t cache_len  = 0;     /* current count of cached numbers */
static idx_t cache_cap  = 0;     /* allocated capacity */

/* Next candidate to test (starts at 2) */
static seq_t next_k = 2;

/* Ensure the cache can hold `needed` elements. */
static void grow_cache(idx_t needed)
{
    if (cache_cap >= needed) return;
    idx_t new_cap = cache_cap == 0 ? 128 : cache_cap * 2;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *new_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_arr) {                    /* allocation failed */
        exit(1);
    }
    cache = new_arr;
    cache_cap = new_cap;
}

/* Main function required by the build. */
seq_t a(idx_t n)
{
    /* n is 1‑based; a(1) is the first term (5). */
    if (n == 0) return 0;              /* undefined, return 0 for safety */

    /* Build the cache up to n if necessary. */
    while (cache_len < n) {
        seq_t k = next_k++;             /* test successive integers */
        if ((k % 3) == 2) {
            if (v2_even(k)) {
                grow_cache(cache_len + 1);
                cache[cache_len++] = k;
            }
        }
    }
    return cache[n - 1];
}
