/*
 * A044030  Numbers k such that 4 and 9 occur juxtaposed in the base-10
 *          representation of k but not of k+1.
 * https://oeis.org/A044030
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th integer k such that the decimal representation
 *            of k contains the adjacent digits 49 or 94, but the decimal
 *            representation of k+1 does not contain 49 or 94. The sequence is
 *            produced by scanning successive integers, checking the property,
 *            and memoising the found terms
 * Range:     u8 n <= 5, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   has_adjacent49or94
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_adjacent49or94.h"

/* Memoised array of found terms.  */
static seq_t *cache = NULL;
static idx_t   cache_cap = 0;
static idx_t   cache_len = 0;           /* number of terms stored */
/* Next candidate integer to inspect.  */
static seq_t   next_to_test = (seq_t)1;

/* Ensure capacity for at least 'needed' elements.  */
static void grow_cache(idx_t needed)
{
    if (needed <= cache_cap) return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();          /* failed allocation */
    cache = tmp;
    cache_cap = new_cap;
}

/* Main sequence function.  */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset is 1, but safe return */

    /* Generate terms until we have at least 'n' terms.  */
    while (cache_len < n) {
        seq_t k = next_to_test;
        if (has_adjacent49or94(k) && !has_adjacent49or94(k + (seq_t)1)) {
            grow_cache(cache_len + 1);
            cache[cache_len++] = k;
        }
        ++next_to_test;
    }
    return cache[n - 1];
}
