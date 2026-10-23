/*
 * A010063  a(n+1) = a(n) + sum of digits in base 3 representation of a(n),
 *          with a(0) = 1.
 * https://oeis.org/A010063
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n+1) = a(n) + sum_of_digits(base 3 of a(n)), a(0) = 1
 * Range:     u8 n <= 54, u16 n <= 6742, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* Computes the OEIS sequence A010063.
 * seq_t: unsigned integer type (8, 16, 32, 64, 128 bits or arbitrary-precision).
 * idx_t: unsigned 64‑bit index type. */
seq_t a(idx_t n)
{
    /* Static cache for memoisation.
     * cache[i] holds a(i).  last is the highest index computed. */
    static seq_t *cache = NULL;
    static size_t   cap   = 0;   /* allocated capacity of cache */
    static idx_t   last   = 0;   /* last computed index */

    /* Initialise cache on first call */
    if (cache == NULL) {
        cap   = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) abort();      /* fatal if RAM exhausted */
        cache[0] = (seq_t)1;             /* a(0) = 1 */
        last   = 0;
    }

    /* If the request is beyond the currently cached range, extend it. */
    if (n > last) {
        for (idx_t i = last + 1; i <= n; ++i) {
            /* Ensure we have enough capacity. */
            if (i >= cap) {
                size_t new_cap = cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (tmp == NULL) abort();
                cache = tmp;
                cap   = new_cap;
            }

            /* Compute a(i) = a(i-1) + digit_sum_base3(a(i-1)) */
            seq_t prev = cache[i - 1];
            seq_t digit_sum = (seq_t)0;
            seq_t temp     = prev;
            /* Sum the digits of prev in base 3 */
            while (temp != (seq_t)0) {
                digit_sum += temp % (seq_t)3;
                temp      /= (seq_t)3;
            }
            cache[i] = prev + digit_sum;
        }
        last = n;
    }

    return cache[n];
}
