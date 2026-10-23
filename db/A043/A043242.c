/*
 * A043242  Numbers k such that 3 and 6 occur juxtaposed in the base-10
 *          representation of k but not of k-1.
 * https://oeis.org/A043242
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   contains_pair
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the nth integer k such that the decimal representation of k
   contains the substrings "36" or "63", while the decimal representation
   of k − 1 does not.  The function generates terms by scanning the
   integers sequentially and caching results. */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/contains_pair.h"

/* Cached sequence values.  The cache is expanded as needed. */
static seq_t *cache = NULL;   /* array of terms */
static idx_t  count = 0;      /* number of terms stored in cache   */
static size_t cap   = 0;      /* current capacity of cache         */
static seq_t  next_cur = 1;   /* next integer to test             */

/* Grow the cache array, doubling its capacity. */
static void grow_cache(void)
{
    if (cap == 0)
        cap = 16;
    else
        cap <<= 1;
    cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
}

/* External linkage as required. */
seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1.  We return 0 for n==0 to
       avoid undefined behaviour, though such a call is beyond the
       OEIS offset. */
    if (n == 0)
        return (seq_t)0;

    /* Generate terms until the cache holds at least n values. */
    while (count < n) {
        bool has_cur     = contains_pair(next_cur);
        seq_t cur_minus1 = next_cur - 1;
        bool has_prev    = contains_pair(cur_minus1);

        if (has_cur && !has_prev) {
            if (count == cap)
                grow_cache();
            cache[count++] = next_cur;
        }
        next_cur++;
    }

    return cache[n - 1];
}
