/*
 * A094686  A Fibonacci convolution.
 * https://oeis.org/A094686
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-2) + 2*a(n-3) + a(n-4) Base values: a(0) = 1, a(1) =
 *            0, a(2) = 1, a(3) = 2, a(4) = 2 This function memoizes the
 *            values to answer subsequent calls efficiently
 * Range:     u8 n <= 13, u16 n <= 25, u32 n <= 48, u64 n <= 94, u128 n <= 186,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache   = NULL;          /* stored values of a(n)            */
static unsigned char *valid = NULL;    /* 1 → value computed, 0 → not yet  */
static idx_t cache_len = 0;            /* number of allocated slots        */

seq_t a(idx_t n)
{
    /* Allocate or grow storage so that index n is valid. */
    idx_t old_len = cache_len;                 /* length before possible growth */
    if (n + 1 > cache_len) {
        idx_t new_len = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) abort();               /* allocation failure */

        unsigned char *new_valid = (unsigned char *)realloc(valid, new_len);
        if (!new_valid) abort();

        /* Initialise the newly allocated part */
        for (idx_t i = old_len; i < new_len; ++i) {
            new_valid[i] = 0;                  /* value not yet computed   */
        }

        cache   = new_cache;
        valid   = new_valid;
        cache_len = new_len;
    }

    /* Compute all terms from old_len up to n (inclusive). */
    for (idx_t i = old_len; i <= n; ++i) {
        if (valid[i]) continue;               /* already computed */

        if (i == 0) {
            cache[i] = (seq_t)1;
        } else if (i == 1) {
            cache[i] = (seq_t)0;
        } else if (i == 2) {
            cache[i] = (seq_t)1;
        } else if (i == 3) {
            cache[i] = (seq_t)2;
        } else if (i == 4) {
            cache[i] = (seq_t)2;
        } else {
            /* Recurrence: a(i) = a(i-2) + 2*a(i-3) + a(i-4) */
            seq_t val  = cache[i - 2];
            seq_t temp = cache[i - 3];
            temp = temp + temp;               /* 2 * a(i-3) */
            val = val + temp;                /* a(i-2) + 2*a(i-3) */
            val = val + cache[i - 4];        /* + a(i-4) */
            cache[i] = val;
        }

        valid[i] = 1;                          /* mark as computed */
    }

    return cache[n];
}
