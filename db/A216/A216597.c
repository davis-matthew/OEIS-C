/*
 * A216597  a(n) = 13*a(n-1) - 65*a(n-2) + 156*a(n-3) - 182*a(n-4) + 91*a(n-5)
 *          - 13*a(n-6), with initial terms 0, -1, -5, -22, -91, -364.
 * https://oeis.org/A216597
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 13*a(n-1) - 65*a(n-2) + 156*a(n-3) - 182*a(n-4) +
 *            91*a(n-5) - 13*a(n-6) initial values: a(0)=0, a(1)=-1, a(2)=-5,
 *            a(3)=-22, a(4)=-91, a(5)=-364
 * Range:     i8 n <= 4, i16 n <= 8, i32 n <= 16, i64 n <= 32, i128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Static cache to store computed terms and flag to mark valid entries */
    static seq_t *cache  = NULL;
    static bool *valid   = NULL;
    static size_t cap    = 0;

    /* Ensure capacity for at least n+1 entries */
    if ((size_t)n >= cap) {
        size_t newcap = cap == 0 ? 64 : cap * 2;
        while ((size_t)n >= newcap) newcap *= 2;
        cache  = (seq_t *)realloc(cache,  newcap * sizeof(seq_t));
        valid  = (bool *)realloc(valid,  newcap * sizeof(bool));
        /* Initialise new entries as invalid */
        for (size_t i = cap; i < newcap; ++i) valid[i] = false;
        cap = newcap;
    }

    /* Initialise the first six terms (idempotent) */
    if (!valid[0]) { cache[0] = (seq_t)0;         valid[0] = true; }
    if (!valid[1]) { cache[1] = (seq_t)(-1);      valid[1] = true; }
    if (!valid[2]) { cache[2] = (seq_t)(-5);      valid[2] = true; }
    if (!valid[3]) { cache[3] = (seq_t)(-22);     valid[3] = true; }
    if (!valid[4]) { cache[4] = (seq_t)(-91);     valid[4] = true; }
    if (!valid[5]) { cache[5] = (seq_t)(-364);    valid[5] = true; }

    /* Compute missing terms up to n */
    for (idx_t i = 6; i <= n; ++i) {
        if (!valid[i]) {
            cache[i] = (13 * cache[i-1]
                        - 65 * cache[i-2]
                        + 156 * cache[i-3]
                        - 182 * cache[i-4]
                        + 91 * cache[i-5]
                        - 13 * cache[i-6]);
            valid[i] = true;
        }
    }

    return cache[n];
}
