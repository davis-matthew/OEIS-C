/*
 * A221643  Numbers k such that k^2 XOR (k+1)^2 is a square, where XOR is the
 *          bitwise XOR operator.
 * https://oeis.org/A221643
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   floor_sqrt, seq_alloc_new_array, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search-based implementation of sequence A221643:
   Numbers k such that k^2 XOR (k+1)^2 is a perfect square. */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/floor_sqrt.h"
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* cache of found terms (k values) */
    static seq_t *cache      = NULL;  /* array of valid k's          */
    static size_t  cache_len = 0;     /* number of terms stored      */
    static size_t  cache_cap = 0;     /* allocated capacity          */
    static seq_t  next_k      = 0;    /* next k to test              */

    if (n == 0) return 0;  /* below the offset, any value works */

    /* grow cache until it holds at least n terms */
    while (cache_len < (size_t)n) {
        seq_t k      = next_k;
        seq_t k_sq   = k * k;
        seq_t kp1_sq = (k + 1) * (k + 1);
        seq_t diff   = k_sq ^ kp1_sq;
        seq_t s      = floor_sqrt(diff);

        if (s * s == diff) {
            /* ensure capacity */
            if (cache_len >= cache_cap) {
                size_t new_cap = (cache_cap == 0 ? 64 : cache_cap * 2);
                if (cache == NULL) {
                    cache = seq_alloc_new_array(new_cap);
                } else {
                    seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                    if (!tmp) exit(EXIT_FAILURE);
                    cache = tmp;
                }
                cache_cap = new_cap;
            }
            cache[cache_len++] = k;
        }
        next_k++;
    }

    return cache[n - 1];
}
