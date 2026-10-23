/*
 * A044029  Numbers k such that 4 and 8 occur juxtaposed in the base-10
 *          representation of k but not of k+1.
 * https://oeis.org/A044029
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the sequence of integers k where the digits 4 and 8 *
 *            occur juxtaposed in the decimal representation of k but not in
 *            k+1
 * Range:     u8 n <= 5, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   has_adjacent48or84
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/has_adjacent48or84.h"

/* Dynamic cache of produced terms. */
static seq_t *cache = NULL;
static idx_t cache_len = 0; /* number of terms stored */
static idx_t cache_cap = 0; /* allocated capacity */
static seq_t next_candidate = 1; /* next integer to test */

static void push_cache(seq_t val)
{
    if (cache_len == cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) abort(); /* allocation failure */
        cache = new_cache;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for n=0, return 0 */

    if (n <= cache_len)
        return cache[n - 1];

    while (cache_len < n) {
        seq_t k = next_candidate;
        for (;;) {
            if (has_adjacent48or84(k)) {
                seq_t kp1 = k + 1;
                if (!has_adjacent48or84(kp1))
                    break;
            }
            k++;
        }
        push_cache(k);
        next_candidate = k + 1;
    }
    return cache[n - 1];
}
