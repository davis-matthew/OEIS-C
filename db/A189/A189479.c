/*
 * A189479  Fixed point starting with 0 of the morphism 0->01, 1->101.
 * https://oeis.org/A189479
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 131, u16 n <= 131, u32 n <= 131, u64 n <= 131,
 *            u128 n <= 131, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A189479: fixed point of the morphism 0->01, 1->101.
   The sequence is generated iteratively; the n-th term is
   returned after the sequence has been grown to length >= n.
   This implementation memoizes the generated prefix so repeated
   calls only expand the array when necessary. */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;      /* cached prefix of the sequence */
static size_t cache_len = 0;     /* number of elements in cache (a(1) .. a(cache_len)) */

seq_t a(idx_t n)
{
    /* n is 1‑based; allocate or grow cache to reach n elements. */
    if (n == 0) {  /* out of range; return 0 as a safe default */
        return 0;
    }

    /* Grow cache if necessary */
    if (n > cache_len) {
        /* If cache empty, start with the first symbol 0 */
        if (cache_len == 0) {
            cache = (seq_t *)malloc(sizeof(seq_t));
            cache[0] = 0;
            cache_len = 1;
        }

        /* Expand the sequence level by level until we reach length >= n */
        while (cache_len < (size_t)n) {
            size_t zeros = 0, ones = 0;
            for (size_t i = 0; i < cache_len; ++i) {
                if (cache[i] == 0) ++zeros;
                else             ++ones;
            }
            size_t next_len = zeros * 2 + ones * 3;
            seq_t *next = (seq_t *)malloc(next_len * sizeof(seq_t));
            size_t idx = 0;
            for (size_t i = 0; i < cache_len; ++i) {
                if (cache[i] == 0) {
                    next[idx++] = 0;
                    next[idx++] = 1;
                } else {
                    next[idx++] = 1;
                    next[idx++] = 0;
                    next[idx++] = 1;
                }
            }
            /* Replace old cache with new */
            free(cache);
            cache = next;
            cache_len = next_len;
        }
    }

    return cache[n - 1];   /* zero‑based access */
}
