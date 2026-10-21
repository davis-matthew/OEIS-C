/*
 * A085895  Group the natural numbers such that the product of the n-th group
 *          is divisible by 2^n. (1), (2), (3,4), (5,6,7,8),
 *          (9,10,11,12,13,14),(15,16,17,18), ... Sequence contains the
 *          product of the terms of the groups.
 * https://oeis.org/A085895
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A085895: product of consecutive integers grouped so that
   the product of the n‑th group is divisible by 2^n.
   The function computes groups lazily and stores the
   product of each finished group. */

#include <stdlib.h>

#ifdef __cplusplus
#include <vector>
#endif

/* Count trailing zeros of an unsigned integer. */
static inline idx_t count_trailing_zeros(idx_t x)
{
    idx_t c = 0;
    while ((x & 1) == 0) {
        ++c;
        x >>= 1;
    }
    return c;
}

seq_t a(idx_t n)
{
    /* State kept between calls */
    static idx_t next_int      = 1;          /* next integer to multiply */
    static idx_t current_group = 0;          /* index of group being built */
    static seq_t current_prod  = 1;          /* product so far in current group */
    static idx_t current_v2    = 0;          /* exponent of 2 in current_prod */

#ifdef __cplusplus
    static std::vector<seq_t> cache;          /* cached a(k) values */
    /* Build enough groups to reach index n */
    while (current_group <= n) {
        seq_t val = static_cast<seq_t>(next_int);
        current_prod *= val;
        current_v2 += count_trailing_zeros(next_int);
        ++next_int;
        if (current_v2 >= current_group) {
            cache.push_back(current_prod);
            current_prod = 1;
            current_v2   = 0;
            ++current_group;
        }
    }
    return cache[n];
#else
    /* C implementation uses a malloced array */
    static seq_t *cache      = NULL;     /* array of cached values */
    static size_t cache_size = 0;        /* allocated size of the array */

    while (current_group <= n) {
        /* Ensure we have room for the next value */
        if (current_group >= cache_size) {
            size_t new_size = cache_size ? cache_size * 2 : 1;
            while (new_size <= current_group) new_size *= 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
            if (!tmp) abort();            /* allocation failure */
            cache     = tmp;
            cache_size = new_size;
        }

        seq_t val = (seq_t)next_int;
        current_prod *= val;
        current_v2 += count_trailing_zeros(next_int);
        ++next_int;
        if (current_v2 >= current_group) {
            cache[current_group] = current_prod;
            current_prod = 1;
            current_v2   = 0;
            ++current_group;
        }
    }
    return cache[n];
#endif
}
