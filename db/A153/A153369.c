/*
 * A153369  Number of zig-zag paths from top to bottom of a rectangle of width
 *          11 with n rows whose color is that of the top right corner.
 * https://oeis.org/A153369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 6*a(n-2) - 9*a(n-4) + 2*a(n-6) Initial values: a(1)=6,
 *            a(2)=10, a(3)=20, a(4)=36, a(5)=72, a(6)=132
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses a simple memoisation table that grows as needed. */
#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    if (n < 1)  /* the sequence is defined only for n ≥ 1 */
        return (seq_t)0;

    /* static cache that keeps the largest value already computed */
    static seq_t *cache = NULL;
    static idx_t max_n = 0;

    /* enlarge the cache if necessary */
    if (n > max_n) {
        idx_t new_cap = n + 1;        /* index 0 is unused */
        seq_t *new_cache = alloc_seq_array(new_cap);

        /* copy the old values */
        if (cache) {
            for (idx_t i = 1; i <= max_n; ++i)
                new_cache[i] = cache[i];
            free_seq_array(cache);
        }

        cache = new_cache;

        /* initialise the first six terms if they are not already set */
        if (max_n < 6) {
            for (idx_t i = max_n + 1; i <= (n < 6 ? n : 6); ++i) {
                switch (i) {
                    case 1: cache[1] = (seq_t)6;   break;
                    case 2: cache[2] = (seq_t)10;  break;
                    case 3: cache[3] = (seq_t)20;  break;
                    case 4: cache[4] = (seq_t)36;  break;
                    case 5: cache[5] = (seq_t)72;  break;
                    case 6: cache[6] = (seq_t)132; break;
                }
            }
        }

        /* compute remaining terms up to n using the recurrence */
        for (idx_t i = max_n + 1; i <= n; ++i) {
            if (i <= 6)  /* already initialised */
                continue;
            cache[i] = (seq_t)6 * cache[i-2]
                     - (seq_t)9 * cache[i-4]
                     + (seq_t)2 * cache[i-6];
        }

        max_n = n;
    }

    return cache[n];
}
