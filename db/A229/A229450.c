/*
 * A229450  Number of 7 X n 0..2 arrays with horizontal differences mod 3
 *          never 1, vertical differences mod 3 never -1, and rows and columns
 *          lexicographically nondecreasing.
 * https://oeis.org/A229450
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-1) - 28*a(n-2) + 56*a(n-3) - 70*a(n-4) + 56*a(n-5)
 *            - 28*a(n-6) + 8*a(n-7) - a(n-8) for n>8 Initial values: a(1)=30,
 *            a(2)=142, a(3)=625, a(4)=2402, a(5)=8024, a(6)=23610,
 *            a(7)=62205, a(8)=149031
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 34, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;      /* allocated size (indices 0..cache_size-1) */
    static size_t max_computed = 0;    /* largest n for which a(n) is known */

    /* base values 1..8 */
    static const seq_t base_vals[8] = {
        (seq_t)30,  (seq_t)142,  (seq_t)625,  (seq_t)2402,
        (seq_t)8024, (seq_t)23610, (seq_t)62205, (seq_t)149031
    };

    /* enlarge cache if needed */
    if ((size_t)n + 1 > cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *new_mem = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (!new_mem) abort();
        if (cache) {
            memcpy(new_mem, cache, cache_size * sizeof(seq_t));
            free(cache);
        } else {
            /* zero out first element, we use 1‑based indexing */
            new_mem[0] = (seq_t)0;
        }
        cache = new_mem;
        cache_size = new_size;
    }

    /* compute missing terms up to n */
    for (size_t i = max_computed + 1; i <= (size_t)n; ++i) {
        if (i <= 8) {
            cache[i] = base_vals[i - 1];
        } else {
            /* Compute a(i) using the recurrence.
               Compute positive and negative parts separately to avoid
               underflow with unsigned arithmetic.  For i >= 9,
               the sum of positive parts is guaranteed to be >= the
               sum of negative parts. */
            seq_t pos = 8 * cache[i - 1];
            pos = pos + (56 * cache[i - 3]);
            pos = pos + (56 * cache[i - 5]);
            pos = pos + (8 * cache[i - 7]);

            seq_t neg = 28 * cache[i - 2];
            neg = neg + (70 * cache[i - 4]);
            neg = neg + (28 * cache[i - 6]);
            neg = neg + cache[i - 8];

            /* pos >= neg for i >= 9, so unsigned subtraction is safe */
            cache[i] = pos - neg;
        }
    }

    max_computed = (size_t)n;
    return cache[n];
}
