/*
 * A039140  Numbers whose base-10 representation has the same number of 3's
 *          and 9's.
 * https://oeis.org/A039140
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   equal_threes_nines
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A039140
   Numbers whose base-10 representation has the same number of 3's and 9's.
   Algorithm: incremental search with memoisation.  The n-th term is
   found by scanning the integers in order, testing each for equal counts
   of digits 3 and 9, and storing the accepted values.  When a larger
   index is requested, the scan continues from where it left off.     */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/equal_threes_nines.h"

static seq_t *cache = NULL;         /* stored terms */
static size_t  cache_len = 0;       /* number of stored terms  */
static size_t  cache_cap = 0;       /* capacity of cache array */

/* Ensure the cache can hold at least new_cap elements. */
static void ensure_capacity(size_t new_cap)
{
    if (new_cap <= cache_cap)
        return;
    size_t cap = cache_cap ? cache_cap * 2 : 4;
    while (cap < new_cap)
        cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, cap * sizeof(seq_t));
    if (!tmp)
        abort();
    cache = tmp;
    cache_cap = cap;
}

/* Generate terms until at least target terms are stored. */
static void generate_to(size_t target)
{
    seq_t cur = (cache_len == 0) ? 0 : cache[cache_len - 1] + 1;
    while (cache_len < target) {
        if (equal_threes_nines(cur)) {
            ensure_capacity(cache_len + 1);
            cache[cache_len++] = cur;
        }
        ++cur;
    }
}

/* Return the n-th term of the sequence (n >= 1). */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;
    size_t target = (size_t)n;
    if (cache_len < target)
        generate_to(target);
    return cache[n - 1];
}
