/*
 * A230312  Squares which cannot be written as the sum of a smaller nonzero
 *          square and twice a triangular number.
 * https://oeis.org/A230312
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = square of the n-th integer m with 4*m^2+1 prime *
 *            (conjecture: a(n) = A001912(n)^2) * a(1)=1, a(2)=4, a(3)=9,
 *            a(4)=25, … (see OEIS A230312)
 * Range:     u8 n <= 5, u16 n <= 41, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_small.h"

seq_t a(idx_t n)
{
    /* Cache of discovered terms, lazily expanded */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static idx_t last_m = 0;          /* last m examined */

    if (n == 0) {          /* offset starts at 1; 0 is undefined */
        return 0;
    }

    /* Expand cache until it contains at least n terms */
    while (cache_len < n) {
        last_m++;                      /* candidate m */

        /* cand = 4*m^2 + 1  (all in seq_t arithmetic) */
        seq_t m = (seq_t)last_m;
        seq_t cand = 4 * m * m + 1;

        if (is_prime_small(cand)) {    /* m satisfies the primality test */
            seq_t sq = m * m;          /* store the square */

            /* Grow cache if necessary */
            if (cache_len >= cache_cap) {
                size_t new_cap = cache_cap ? cache_cap << 1 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = sq;
        }
    }

    return cache[n - 1];
}
