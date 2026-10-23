/*
 * A037335  Numbers whose base-8 and base-10 expansions have the same digit
 *          sum.
 * https://oeis.org/A037335
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers whose base-8 and base-10 expansions have the same digit sum. */
/* The function returns the n-th such number, with a(1)=1. */

#include <stdlib.h>
#include "../utils/digit_sum.h"

static seq_t *cache = NULL;      /* cached sequence values */
static idx_t cache_size = 0;     /* number of values currently cached */
static idx_t cache_cap = 0;      /* allocated capacity of the cache array */
static seq_t last_val = 0;       /* last value that was generated */

/* Ensure that cache contains at least the first n terms of the sequence */
static void ensure_cache(idx_t n)
{
    if (n <= cache_size) return;          /* already satisfied */

    /* Allocate initial cache if not yet done */
    if (cache == NULL) {
        cache_cap = (n > 100 ? n : 100);
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    seq_t val = last_val + 1;             /* value to test next */
    idx_t count = cache_size;             /* how many terms we have now */

    while (count < n) {
        /* Compute 8‑ary digit sum */
        seq_t tmp = val;
        seq_t sum8 = 0;
        while (tmp > 0) {
            sum8 += tmp & 7;              /* tmp % 8, but faster */
            tmp >>= 3;                    /* tmp /= 8 */
        }

        /* Compare with decimal digit sum (utility function) */
        if (digit_sum(val) == sum8) {
            /* Store candidate into the cache, expanding if needed */
            if (count >= cache_cap) {
                cache_cap *= 2;
                seq_t *new_cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                cache = new_cache;
            }
            cache[count] = val;
            ++count;
        }
        val = val + 1;                    /* next integer to test */
    }

    last_val   = val - 1;                 /* last tested value */
    cache_size = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined below offset, but harmless */

    ensure_cache(n);
    return cache[n - 1];
}
