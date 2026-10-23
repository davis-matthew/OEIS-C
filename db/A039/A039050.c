/*
 * A039050  Numbers whose base-8 representation has the same number of 0's and
 *          1's.
 * https://oeis.org/A039050
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   good_base8
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: numbers whose base‑8 representation has the same number of 0’s and 1’s. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/good_base8.h"

/* static cache for generated terms */
static seq_t *cache = NULL;
static idx_t cache_size = 0;
static idx_t cache_cap = 0;
static seq_t next_candidate = 1;

/* grow cache capacity to at least `need` elements */
static void ensure_capacity(idx_t need)
{
    if (need <= cache_cap) return;
    idx_t new_cap = cache_cap ? cache_cap : 8;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();   /* allocation failure */
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset is 1; guard for safety */
    while (cache_size < n) {
        if (good_base8(next_candidate)) {
            ensure_capacity(cache_size + 1);
            cache[cache_size++] = next_candidate;
        }
        next_candidate++;
    }
    return cache[n - 1];
}
