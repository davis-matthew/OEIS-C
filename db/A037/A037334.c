/*
 * A037334  Numbers whose base-8 and base-9 expansions have the same digit
 *          sum.
 * https://oeis.org/A037334
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A037334: Numbers whose base-8 and base-9 digit sums are equal.  
 * The algorithm scans positive integers in increasing order, tests the
 * digit‑sum condition for each, and stores the first n occurrences in a
 * static cache so that subsequent calls are fast.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Computes the sum of the digits of x in the specified base (8 or 9). */
static inline seq_t digit_sum_in_base(seq_t x, unsigned base)
{
    seq_t sum = 0;
    while (x != 0) {
        sum += x % base;
        x   /= base;
    }
    return sum;
}

/* External linkage as required. */
seq_t a(idx_t n)
{
    if (n == 0)                /* undefined for offset 1, but return 0 */
        return 0;

    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;      /* highest index stored + 1 */
    static size_t cache_cap = 0;      /* allocated capacity */
    static seq_t current = 1;         /* next integer to test */

    /* Grow cache if it will hold the new element */
    if (cache_len >= n && n <= cache_len) {
        return cache[n - 1];
    }

    /* Extend cache until it contains the nth element */
    while (cache_len < n) {
        /* Test the current integer */
        seq_t sum8 = digit_sum_in_base(current, 8);
        seq_t sum9 = digit_sum_in_base(current, 9);

        if (sum8 == sum9) {
            /* Ensure sufficient capacity */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *new_ptr = (seq_t *)malloc(new_cap * sizeof(seq_t));
                if (cache) {
                    for (size_t i = 0; i < cache_len; ++i)
                        new_ptr[i] = cache[i];
                    free(cache);
                }
                cache = new_ptr;
                cache_cap = new_cap;
            }
            cache[cache_len++] = current;
        }
        ++current;   /* move to next integer */
    }

    /* Now cache[n-1] is available */
    return cache[n - 1];
}
