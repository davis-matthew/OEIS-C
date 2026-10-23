/*
 * A164140  Number of binary strings of length n with equal numbers of 000 and
 *          101 substrings.
 * https://oeis.org/A164140
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 35, u64 n <= 67, u128 n <= 132,
 *            big any n
 * Helpers:   one_bit_prev, two_bits_prev
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A164140 – Number of binary strings of length n with equal
   numbers of 000 and 101 substrings.  The function builds the result
   incrementally with a dynamic programming over the last two bits and
   the difference between the two substrings.  It caches all computed
   values so any subsequent call is answered immediately. */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/one_bit_prev.h"
#include "../utils/two_bits_prev.h"

/* --- Private helpers ------------------------------------------------- */

static void
init_cache(void)
{
    static int initialized = 0;
    if (initialized) return;
    initialized = 1;
}

/* --- Main function ---------------------------------------------------- */

seq_t
a(idx_t n)
{
    /* Static cache structure */
    static seq_t *cache = NULL;         /* a[0..max_n] */
    static size_t cache_len = 0;         /* allocated size */
    static size_t cached_n = 0;          /* largest n computed */

    static seq_t *dp_cur[4] = {NULL, NULL, NULL, NULL};
    static idx_t cur_len = 0;            /* current length represented by dp_cur */

    /* Basic cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)4;
    if (n == 3) {
        /* a(3) = 6 – handled by DP below, but we memoise the first term. */
    }

    /* Zero or negative states are irrelevant; use static init check. */
    init_cache();

    /* Allocate initial cache for n=0..2 */
    if (cache_len == 0) {
        cache_len = 3;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_len);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)4;
        cached_n = 2;
    }

    /* Return cached value if already computed */
    if (n <= cached_n) {
        return cache[n];
    }

    /* Initialise DP structures for length 2 if not already done */
    if (cur_len == 0) {
        cur_len = 2;
        int offset = 0;
        size_t sz = 1;
        for (int s = 0; s < 4; ++s) {
            dp_cur[s] = (seq_t *)malloc(sizeof(seq_t) * sz);
            dp_cur[s][0] = (seq_t)1;
        }
    }

    /* Extend computation up to n */
    for (idx_t target = cached_n + 1; target <= n; ++target) {
        /* Current length is cur_len, next length will be cur_len+1 */
        idx_t old_len = cur_len;
        idx_t new_len = old_len + 1;

        int offset_old = (int)old_len - 2;          /* range for old len */
        size_t size_old = (size_t)(2 * offset_old + 1);

        int offset_new = (int)new_len - 2;          /* range for new len */
        size_t size_new = (size_t)(2 * offset_new + 1);

        /* Allocate arrays for next DP stage and zero them */
        seq_t *dp_next[4];
        for (int s = 0; s < 4; ++s) {
            dp_next[s] = (seq_t *)calloc(size_new, sizeof(seq_t));
        }

        /* Transition from dp_cur to dp_next */
        for (int s = 0; s < 4; ++s) {
            int a_bit = two_bits_prev(s);
            int b_bit = one_bit_prev(s);
            for (size_t i = 0; i < size_old; ++i) {
                seq_t val = dp_cur[s][i];
                if (val == 0) continue;
                int d = (int)i - offset_old;
                for (int bit = 0; bit < 2; ++bit) {
                    int delta = 0;
                    if (a_bit == 0 && b_bit == 0 && bit == 0) {
                        delta = 1;                  /* 000 occurs */
                    } else if (a_bit == 1 && b_bit == 0 && bit == 1) {
                        delta = -1;                 /* 101 occurs */
                    }
                    int new_d = d + delta;
                    int new_i = new_d + offset_new;
                    dp_next[(b_bit << 1) | bit][(size_t)new_i] += val;
                }
            }
        }

        /* Free old DP arrays */
        for (int s = 0; s < 4; ++s) free(dp_cur[s]);

        /* Adopt new DP arrays */
        for (int s = 0; s < 4; ++s) dp_cur[s] = dp_next[s];
        cur_len = new_len;

        /* Sum counts with difference zero to obtain a[cur_len] */
        seq_t sum = 0;
        for (int s = 0; s < 4; ++s) {
            sum += dp_cur[s][offset_new];
        }

        /* Extend cache if necessary */
        if (cache_len <= (size_t)target) {
            size_t new_cache_size = target + 1;
            cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_cache_size);
            cache_len = new_cache_size;
        }
        cache[target] = sum;
        cached_n = target;
    }

    return cache[n];
}
