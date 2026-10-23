/*
 * A044033  Numbers k such that 5 and 8 occur juxtaposed in the base-10
 *          representation of k but not of k+1.
 * https://oeis.org/A044033
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = k such that 58 or 85 occurs in k but not in k+1 using a
 *            search over all integers
 * Range:     u8 n <= 4, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   has58or85
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has58or85.h"

static seq_t *cache = NULL;        /* array of found terms           */
static size_t cache_len = 0;       /* number of terms stored         */
static size_t cache_cap = 0;       /* allocated capacity             */
static seq_t next_candidate = 0;   /* next integer to test           */

static void ensure_capacity(size_t required)
{
    if (required <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < required)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!tmp) {
        /* In a real implementation you might abort, but for the
           purposes of the exercise returning 0 is sufficient. */
        return;
    }
    for (size_t i = 0; i < cache_len; ++i)
        tmp[i] = cache[i];
    free(cache);
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* out of bounds, but harmless */
    size_t idx = (size_t)n;
    if (idx <= cache_len)
        return cache[idx - 1];

    /* generate terms until we reach the desired index */
    while (cache_len < idx) {
        next_candidate++;
        if (has58or85(next_candidate) && !has58or85(next_candidate + 1)) {
            ensure_capacity(cache_len + 1);
            cache[cache_len++] = next_candidate;
        }
    }
    return cache[idx - 1];
}
