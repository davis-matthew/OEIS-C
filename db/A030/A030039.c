/*
 * A030039  a(n+1) = Sum_{k=0..floor(4*n/5)} a(k) * a(n-k).
 * https://oeis.org/A030039
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 27, u64 n <= 49, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A030039
   a(0) = 1
   a(n) = sum_{k=0..floor(4*(n-1)/5)} a(k) * a(n-1-k)   for n >= 1
   Offset: 0
   Computes each term once and memoises the values for arbitrary call order. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache: values already computed and stored */
    static seq_t *cache = NULL;      /* cache[0] = a(0), cache[1] = a(1), ... */
    static idx_t cache_len = 0;      /* number of values stored (d+1) */

    /* Grow cache to include n */
    if (n >= cache_len) {
        /* enlarge the array to hold n+1 terms */
        idx_t new_len = n + 1;
        /* realloc handles NULL pointer == malloc */
        cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        /* initialize the values from current length up to n */
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i == 0) {
                cache[0] = 1;          /* base case */
            } else {
                /* compute a(i) using the recurrence */
                idx_t limit = (4 * (i - 1)) / 5;   /* floor(4*(i-1)/5) */
                seq_t sum = 0;
                for (idx_t k = 0; k <= limit; ++k) {
                    seq_t prod = cache[k] * cache[i - 1 - k];
                    sum += prod;
                }
                cache[i] = sum;
            }
        }
        cache_len = new_len;
    }

    return cache[n];
}
