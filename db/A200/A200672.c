/*
 * A200672  Partial sums of A173862.
 * https://oeis.org/A200672
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-3) + 3 for n > 3, with a(1)=1, a(2)=2, a(3)=3
 *            (Partial sums of A173862)
 * Range:     u8 n <= 19, u16 n <= 43, u32 n <= 91, u64 n <= 187,
 *            u128 n <= 379, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache for previously computed terms */
    static seq_t *cache = NULL;
    static size_t cap = 0;   /* allocated size of cache (+1 for index 0) */
    static size_t len = 0;   /* highest index that has been computed */

    if (n == 0)                /* undefined for n<1, return 0 safely */
        return 0;

    size_t target = (size_t)n; /* n is unsigned so safe cast */

    /* initialise cache if first call */
    if (cache == NULL) {
        cap = 8;                       /* start with some space */
        cache = (seq_t*)calloc(cap, sizeof(seq_t));
        len = 0;
    }

    /* compute all values up to target */
    while (len < target) {
        size_t next = len + 1;        /* next index to fill */

        /* ensure we have room for the new entry */
        if (next >= cap) {
            size_t new_cap = cap ? cap * 2 : 8;
            if (new_cap <= next)
                new_cap = next + 1;
            cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            cap = new_cap;
        }

        /* fill a(next) */
        if (next == 1)
            cache[next] = (seq_t)1;
        else if (next == 2)
            cache[next] = (seq_t)2;
        else if (next == 3)
            cache[next] = (seq_t)3;
        else
            cache[next] = cache[next - 3] * (seq_t)2 + (seq_t)3;

        len = next;
    }

    return cache[target];
}
