/*
 * A032953  Numbers whose base-3 representation Sum_{i=0..m} d(i)*3^(m-i) has
 *          even d(i) for all odd i.
 * https://oeis.org/A032953
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 120, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   valid_b3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A032953 – numbers whose base‑3 representation has even digits at
   all odd positions (counting from the most significant digit, i.e. d(0)).
   The function implements a brute‑force search with memoized results. */ 
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/valid_b3.h"

/* Static cache of computed terms. */
static seq_t *cache          = NULL;   /* array holding sequence values */
static idx_t cache_len       = 0;      /* number of values stored */
static idx_t cache_cap       = 0;      /* current capacity of the array */
static seq_t next_candidate  = 1;      /* smallest integer yet unchecked */

/* Return the n-th term (a(1)=1).  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* out of offset – undefined, guard */

    /* Ensure the cache contains at least n values. */
    if (n > cache_len) {
        /* Grow the cache if necessary. */
        if (cache_cap < n) {
            idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
            while (new_cap < n) new_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();   /* fatal allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Generate additional terms until we have n of them. */
        while (cache_len < n) {
            if (valid_b3(next_candidate))
                cache[cache_len++] = next_candidate;
            ++next_candidate;
        }
    }
    return cache[n - 1];
}
