/*
 * A230567  Number of n X 4 0..4 arrays x(i,j) with each element horizontally
 *          or antidiagonally next to at least one element with value
 *          (x(i,j)+1) mod 5, and upper left element zero.
 * https://oeis.org/A230567
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-3) + a(n-4) + a(n-5) + a(n-6) +
 *            a(n-7) a(n) = 17*a(n-1) - 66*a(n-2) - 12*a(n-3) + 13*a( n-4) +
 *            476*a(n-5) - 44*a(n-6) + a(n-7) (Empirical recurrence of order 7
 *            with initial values a(1)=0, a(2)=0, a(3)=2, a(4)=32, a(5)=430,
 *            a(6)=5130, a(7)=58474.)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mul_small.h"

/* Forward declaration of seq_t and idx_t defined by the build */
seq_t a(idx_t n) {
    /* Static cache to store computed values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0; /* last index that has a valid value */

    /* Allocate or extend cache if necessary */
    if (n > cache_size) {
        idx_t new_size = n + 1; /* store up to index n */
        seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
        if (!new_cache) {
            /* Allocation failed – terminate with error */
            abort();
        }
        cache = new_cache;
        /* Fill cache from cache_size+1 up to n */
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            if (i == 1) { cache[1] = 0; }
            else if (i == 2) { cache[2] = 0; }
            else if (i == 3) { cache[3] = 2; }
            else if (i == 4) { cache[4] = 32; }
            else if (i == 5) { cache[5] = 430; }
            else if (i == 6) { cache[6] = 5130; }
            else if (i == 7) { cache[7] = 58474; }
            else {
                /* Use the recurrence:
                 * a(n) = 17*a(n-1) - 66*a(n-2) - 12*a(n-3) + 13*a(n-4) + 476*a(n-5)
                 *        - 44*a(n-6) + a(n-7)
                 *
                 * To avoid problems with unsigned subtraction of negative terms,
                 * compute the sum of positive contributions and subtract the
                 * sum of negative contributions afterwards.  The recurrence
                 * guarantees that the positive sum is never smaller than the
                 * negative sum, so the unsigned subtraction is safe. */
                seq_t a1 = cache[i - 1]; /* a(n-1) */
                seq_t a2 = cache[i - 2]; /* a(n-2) */
                seq_t a3 = cache[i - 3]; /* a(n-3) */
                seq_t a4 = cache[i - 4]; /* a(n-4) */
                seq_t a5 = cache[i - 5]; /* a(n-5) */
                seq_t a6 = cache[i - 6]; /* a(n-6) */
                seq_t a7 = cache[i - 7]; /* a(n-7) */

                /* Positive part: 17*a1 + 13*a4 + 476*a5 + a7 */
                seq_t pos = 0;
                pos += mul_small(a1, 17);
                pos += mul_small(a4, 13);
                pos += mul_small(a5, 476);
                pos += a7;

                /* Negative part: 66*a2 + 12*a3 + 44*a6 */
                seq_t neg = 0;
                neg += mul_small(a2, 66);
                neg += mul_small(a3, 12);
                neg += mul_small(a6, 44);

                cache[i] = pos;
                cache[i] -= neg; /* guaranteed to be non‑negative */
            }
        }
        cache_size = n;
    }
    return cache[n];
}
