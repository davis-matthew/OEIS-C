/*
 * A203976  a(n) = 3*a(n-2) - a(n-4), a(0)=0, a(1)=1, a(2)=5, a(3)=4.
 * https://oeis.org/A203976
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=1, a(2)=5, a(3)=4 * Recurrence: a(n) = 3*a(n-2) -
 *            a(n-4) (for n ≥ 4)
 * Range:     u8 n <= 9, u16 n <= 21, u32 n <= 45, u64 n <= 91, u128 n <= 183,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Initial values for small n */
    if (n < 4) {
        switch ((int)n) {
            case 0: return 0;
            case 1: return 1;
            case 2: return 5;
            case 3: return 4;
        }
    }

    static seq_t *cache = NULL;
    static idx_t next_index = 0;   /* next index to compute (size of cache) */
    static idx_t cap = 0;          /* current capacity */

    if (cache == NULL) {
        cap = 8; /* initial capacity for caching */
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache) exit(1);        /* allocation failed */
        cache[0] = 0;
        cache[1] = 1;
        cache[2] = 5;
        cache[3] = 4;
        next_index = 4;
    }

    if (n >= next_index) {
        for (idx_t i = next_index; i <= n; ++i) {
            /* compute a(i) = 3*a(i-2) - a(i-4) */
            seq_t val = 3 * cache[i - 2] - cache[i - 4];

            if (i >= cap) {
                cap *= 2;
                cache = (seq_t*)realloc(cache, cap * sizeof(seq_t));
                if (!cache) exit(1); /* reallocation failed */
            }
            cache[i] = val;
        }
        next_index = n + 1;
    }

    return cache[n];
}
