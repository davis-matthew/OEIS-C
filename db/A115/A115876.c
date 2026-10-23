/*
 * A115876  Integers i such that 41*i = 105 X i.
 * https://oeis.org/A115876
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 8, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A115876
   Computation: integers i such that 41*i  =  105 X i,
   where  X denotes carryless (GF(2)[X]) multiplication.
   105 in binary has bits 0,3,5,6, so
      105 X i = i ^ (i<<3) ^ (i<<5) ^ (i<<6).
   For each candidate i we test whether
      (seq_t)41 * i == i ^ (i<<3) ^ (i<<5) ^ (i<<6).
   The function a(n) returns the (n+1)-st such integer
   (a(0)=0, a(1)=7, …).  The sequence is generated lazily
   and memoised in a static array so that calls for arbitrary
   n come in any order.  The implementation uses only
   integer arithmetic allowed by the problem statement.
*/

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;      /* number of stored terms */
static idx_t  checked_up_to = 0;  /* last i that was checked */

/* grow the cache to at least new_cap elements */
static void grow_cache(size_t new_cap) {
    if (new_cap <= cache_cap)
        return;
    size_t new_size = new_cap > 16 ? new_cap : 16;
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    cache = new_cache;
    cache_cap = new_size;
}

seq_t a(idx_t n) {
    /* initialise cache on first call */
    if (cache_len == 0) {
        grow_cache(16);
        cache[0] = (seq_t)0;   /* a(0) = 0 */
        cache_len = 1;
        checked_up_to = 0;
    }

    /* expand cache until it contains term a(n) */
    while (cache_len <= (size_t)n) {
        /* evaluate next candidate */
        checked_up_to++;
        seq_t i = (seq_t)checked_up_to;

        seq_t lhs = ((seq_t)41) * i;
        seq_t rhs = i ^ (i << 3) ^ (i << 5) ^ (i << 6);

        if (lhs == rhs) {
            /* add to cache */
            grow_cache(cache_len + 1);
            cache[cache_len++] = i;
        }
    }

    return cache[(size_t)n];
}
