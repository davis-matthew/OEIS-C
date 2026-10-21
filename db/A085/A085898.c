/*
 * A085898  Group the natural numbers such that the product of the n-th group
 *          is divisible by 2^n. (1), (2), (3,4), (5,6,7,8),
 *          (9,10,11,12,13,14), (15,16,17,18), ... Sequence contains highest
 *          power of 2 that divides the product of the terms of the n-th
 *          group. a(n) = k, where A085895(n)/(2^k) is the smallest...
 * https://oeis.org/A085898
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the exponent of 2 in the product of the minimal *
 *            consecutive block of natural numbers that follows the previous *
 *            block and contains at least n factors of 2. (The block may
 *            contain * more than n factors of 2.)
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Extra includes needed for types and macros used below. */
#include <stdint.h>
#include <stdlib.h>

/* Integer type for sequence values; defined by the build system.     */
/* Integer type for indices; defined to be uint64_t by the build.     */

/* Forward declarations of static state variables. */
static seq_t *cache = NULL;          /* dynamic array of a(n) values      */
static size_t cache_cap = 0;         /* allocated capacity in cache      */
static size_t cache_used = 0;        /* number of stored values (n+1)    */
static idx_t  next_num = 1;          /* next natural number to try       */
static idx_t  curr_sum = 0;          /* current cumulative v2 in group   */

/* Compute exponent of 2 in an unsigned integer x. */
static inline idx_t v2_idx(idx_t x)
{
    idx_t cnt = 0;
    while ((x & 1) == 0) {
        x >>= 1;
        cnt++;
    }
    return cnt;
}

/* Grow the cache array to at least new_cap entries. */
static void ensure_cache_cap(size_t new_cap)
{
    if (new_cap <= cache_cap)
        return;
    size_t cap = cache_cap ? cache_cap : 16;
    while (cap < new_cap)
        cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = cap;
}

/* Initialise static state on first use. */
static void init_cache(void)
{
    if (cache != NULL)
        return;
    ensure_cache_cap(32);
    cache[0] = 0;             /* a(0) = 0                                 */
    cache_used = 1;           /* next index that needs filling is 1        */
    next_num = 1;             /* start scanning natural numbers from 1     */
    curr_sum = 0;             /* no numbers in the current (empty) group  */
}

/* Return a(n).  All indices are 0‑based. */
seq_t a(idx_t n)
{
    init_cache();

    /* Compute missing terms up to n. */
    while (cache_used <= n) {
        idx_t target = cache_used;      /* group index to compute        */

        /* Accumulate v2 exponents until the sum reaches the target. */
        while (curr_sum < target) {
            idx_t v = v2_idx(next_num);
            curr_sum += v;
            next_num++;
        }

        /* Store the result for this group. */
        ensure_cache_cap(cache_used + 1);
        cache[cache_used] = (seq_t)curr_sum;
        cache_used++;

        /* Reset the counter for the next group. */
        curr_sum = 0;
    }

    return cache[n];
}
