/*
 * A052454  Positive integer values of k such that 10*k^2 - 9 is a square.
 * https://oeis.org/A052454
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 37, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A052454: k such that 10*k^2-9 is a square
 * Recurrence: a(n) = 38*a(n-3) - a(n-6)
 * Initial values:
 *   a(1)=1, a(2)=3, a(3)=13, a(4)=25, a(5)=111, a(6)=493
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache: 1‑based */
    static seq_t *cache = NULL;
    static idx_t max_computed = 0;   /* highest index whose value is stored */

    if (n == 0) return (seq_t)0;    /* not used, but avoid UB */

    /* Ensure we have the base values stored before any computation */
    if (max_computed == 0) {
        /* Allocate at least 6 entries (index 0 unused) */
        cache = (seq_t*)realloc(cache, 7 * sizeof(seq_t));
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)13;
        cache[4] = (seq_t)25;
        cache[5] = (seq_t)111;
        cache[6] = (seq_t)493;
        max_computed = 6;
    }

    /* Grow cache and compute new terms if needed */
    if ((idx_t)n > max_computed) {
        /* Reallocate to size n+1 (index 0 unused) */
        seq_t *tmp = (seq_t*)realloc(cache, ((size_t)n + 1) * sizeof(seq_t));
        cache = tmp;

        for (idx_t i = max_computed + 1; i <= n; ++i) {
            if (i <= 6) {
                /* already initialized above */
                continue;
            }
            cache[i] = (seq_t)38 * cache[i - 3] - cache[i - 6];
        }
        max_computed = (idx_t)n;
    }

    return cache[n];
}
