/*
 * A022488  Describe previous term from the right (method B - initial term is
 *          2).
 * https://oeis.org/A022488
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   next_string, seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Description: a(1)=2; for n>1 a(n) is obtained by describing the terms of a(n-1)
 * from the right: group consecutive equal digits, and for each group write
 * the digit followed by the run length. This is method B in OEIS A022488. */

#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "../utils/seq_from_str.h"
#include "../utils/next_string.h"

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t current_max = 0;
static char *last_str = NULL;
static size_t last_len = 0;

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined, but keep safe */

    /* initialize the first term if not yet done */
    if (current_max == 0) {
        /* a(1) = 2 */
        seq_t v = 2;
        if (cache_cap == 0) {
            cache_cap = 8;
            cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        }
        cache[0] = v;
        cache_len = 1;
        current_max = 1;
        last_len = 1;
        last_str = (char *)malloc(2);
        last_str[0] = '2';
        last_str[1] = '\0';
    }

    /* compute missing terms up to n */
    for (idx_t i = current_max + 1; i <= n; ++i) {
        size_t new_len;
        char *new_str = next_string(last_str, last_len, &new_len);
        seq_t val = seq_from_str(new_str);
        /* store in cache */
        if (cache_len == cache_cap) {
            cache_cap = cache_cap * 2 + 8;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }
        cache[cache_len++] = val;
        /* free the old string and prepare for next iteration */
        free(last_str);
        last_str = new_str;
        last_len = new_len;
        current_max = i;
    }

    return cache[n - 1];
}
