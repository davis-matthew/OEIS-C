/*
 * A015926  Positive integers n such that 2^n == 2^6 (mod n).
 * https://oeis.org/A015926
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth positive integer m such that 2^m ≡ 2^6 (mod m)
 * Range:     u8 n <= 38, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/mod_pow.h"

static seq_t *cache = NULL;          /* stored sequence values */
static size_t cache_size = 0;        /* number of values stored */
static size_t cache_cap   = 0;        /* allocated capacity of cache */
static uint64_t last_checked = 0;    /* largest integer examined so far */

static void grow_cache_once(void)
{
    size_t new_cap = (cache_cap == 0 ? 8 : cache_cap * 2);
    seq_t *new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_arr) return;         /* silently fail – caller will abort */
    if (cache)
        memcpy(new_arr, cache, cache_size * sizeof(seq_t));
    free(cache);
    cache = new_arr;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for index 0, just return 0 */

    while (cache_size < (size_t)n) {
        uint64_t cand = last_checked + 1;
        last_checked = cand;

        seq_t val = mod_pow((seq_t)2, cand, (seq_t)cand);
        seq_t rhs = ((seq_t)64) % (seq_t)cand;

        if (val == rhs) {
            if (cache_size + 1 > cache_cap)
                grow_cache_once();
            cache[cache_size] = (seq_t)cand;
            cache_size++;
        }
    }
    return cache[n - 1];
}
