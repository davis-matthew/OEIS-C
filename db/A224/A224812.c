/*
 * A224812  Number of subsets of {1,2,...,n-10} without differences equal to
 *          2, 4, 6, 8 or 10.
 * https://oeis.org/A224812
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of subsets of {1,…,n-10} with no pairwise
 *            difference equal to 2,4,6,8 or 10. Computed by dynamic
 *            programming on a 10‑bit mask representing the inclusion status
 *            of the last 10 positions
 * Range:     u8 n <= 27, u16 n <= 50, u32 n <= 94, u64 n <= 182,
 *            u128 n <= 359, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

#define MASK_BITS           10
#define MASK_SIZE           (1 << MASK_BITS)
#define MASK_MASK_MINUS1    (MASK_SIZE - 1)
#define FORBID_MASK         0x02AA   /* bits 1,3,5,7,9 set */

static seq_t *cache          = NULL;   /* cache[n] = a(n) */
static size_t cache_capacity = 0;      /* allocated size of cache */
static idx_t  max_cached_n   = 0;      /* highest n computed (>=10) */

static seq_t dp_cur[MASK_SIZE];        /* states after processing N elements */
static seq_t dp_next[MASK_SIZE];       /* temporary for next step */
static bool   dp_initialized = false;

/* Zero a 1024‑entry array */
static void zero_array(seq_t *arr)
{
    for (int i = 0; i < MASK_SIZE; ++i)
        arr[i] = 0;
}

seq_t a(idx_t n)
{
    /* The first 11 terms are all 1 by definition. */
    if (n <= 10)
        return (seq_t)1;

    /* One‑time initialization of cache and DP. */
    if (!cache) {
        cache_capacity = (size_t)(n + 1);
        cache = (seq_t *)malloc(cache_capacity * sizeof(seq_t));
        if (!cache)
            return (seq_t)0;          /* allocation failure */

        /* Initialize cache[0..10] = 1. */
        for (idx_t i = 0; i <= 10 && i < cache_capacity; ++i)
            cache[i] = (seq_t)1;
        for (idx_t i = 11; i < cache_capacity; ++i)
            cache[i] = (seq_t)0;

        /* Initialise DP for N = 0. */
        zero_array(dp_cur);
        dp_cur[0] = (seq_t)1;
        zero_array(dp_next);
        dp_initialized = true;
        max_cached_n   = 10;
    }

    /* Grow cache if needed. */
    if (cache_capacity <= (size_t)n) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;          /* reallocation failure */
        /* Initialise newly allocated slots. */
        for (size_t i = cache_capacity; i < new_cap; ++i)
            tmp[i] = (seq_t)0;
        cache = tmp;
        cache_capacity = new_cap;
    }

    /* Extend DP up to requested n. */
    while (max_cached_n < n) {
        zero_array(dp_next);
        for (int mask = 0; mask < MASK_SIZE; ++mask) {
            seq_t val = dp_cur[mask];
            if (!val)
                continue;
            int shift = (mask << 1) & MASK_MASK_MINUS1; /* exclude current */

            /* Option 1: do not include current element */
            dp_next[shift] += val;

            /* Option 2: include current element if allowed */
            if ((mask & FORBID_MASK) == 0) {
                int newmask = shift | 1;
                dp_next[newmask] += val;
            }
        }
        /* Make dp_next the current state for the next iteration */
        for (int i = 0; i < MASK_SIZE; ++i)
            dp_cur[i] = dp_next[i];

        /* Sum all states to get a(n) for this n */
        seq_t sum = 0;
        for (int i = 0; i < MASK_SIZE; ++i)
            sum += dp_cur[i];

        cache[max_cached_n + 1] = sum;
        max_cached_n++;
    }

    return cache[n];
}
