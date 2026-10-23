/*
 * A044878  Numbers having, in base 7, (sum of even run lengths)=(sum of odd
 *          run lengths).
 * https://oeis.org/A044878
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   to_base7_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A044878: Numbers whose base‑7 representation has runs of identical digits
   such that the sum of the lengths of runs with even length equals the sum of the
   lengths of runs with odd length.
   The implementation collects such numbers in increasing order and returns the
   n‑th one (offset 1).  For efficiency it uses a simple incremental search and
   caches all found terms so that repeated calls are fast.
*/
#include <stdint.h>
#include <stdlib.h>
#include "../utils/to_base7_uint64.h"

/* Helper: test whether a number satisfies the run‑length condition */
static int is_target(uint64_t x)
{
    unsigned char digits[20];
    size_t len;
    to_base7_uint64(x, digits, &len);

    size_t sum_even = 0, sum_odd = 0;
    size_t run = 1;
    for (size_t i = 1; i < len; ++i) {
        if (digits[i] == digits[i - 1]) {
            ++run;
        } else {
            if ((run & 1) == 0)
                sum_even += run;
            else
                sum_odd += run;
            run = 1;
        }
    }
    /* last run */
    if ((run & 1) == 0)
        sum_even += run;
    else
        sum_odd += run;

    return sum_even == sum_odd;
}

/* Cache of found terms */
static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap = 0;
static uint64_t next_candidate = 0;

/* Expand the cache when needed */
static void grow_cache(void)
{
    size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort(); /* OOM: give up */
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined, return 0 */

    /* Ensure cache has at least n values */
    while (cache_size < (size_t)n) {
        if (is_target(next_candidate)) {
            if (cache_size == cache_cap)
                grow_cache();
            cache[cache_size++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
    return cache[(size_t)n - 1];
}
