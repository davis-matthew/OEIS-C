/*
 * A043488  Numbers having four 8's in base 9.
 * https://oeis.org/A043488
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 31, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   count_eights_in_base9
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes numbers having exactly four 8's in base‑9 representation
   by scanning successive integers and caching those that satisfy the
   property.  The function returns the n‑th such number (n ≥ 1). */
#include <stdlib.h>
#include <stddef.h>
#include "../utils/count_eights_in_base9.h"

static seq_t *cache = NULL;          /* cached terms of the sequence */
static size_t cache_len = 0;           /* number of terms cached so far */
static size_t cache_cap = 0;           /* capacity of the cache array       */
static idx_t last_num = 0;             /* last integer tested (starts at 0) */

seq_t a(idx_t n)
{
    if (n == 0)      /* offset is 1; behaviour for n==0 is undefined */
        return 0;

    /* Grow the cache until we have at least n terms stored.     */
    while (cache_len < n) {
        ++last_num;
        if (count_eights_in_base9(last_num) == 4) {
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap) ? (cache_cap << 1) : 8;
                seq_t *tmp = (seq_t *)realloc((void *)cache,
                                            new_cap * sizeof(seq_t));
                if (!tmp) {
                    /* Allocation failed; return zero as error value. */
                    return 0;
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)last_num;
        }
    }

    /* Return the (n‑1)‑indexed element of the cache. */
    return cache[n - 1];
}
