/*
 * A079496  a(0) = a(1) = 1; thereafter a(2*n+1) = 2*a(2*n) - a(2*n-1), a(2*n)
 *          = 4*a(2*n-1) - a(2*n-2).
 * https://oeis.org/A079496
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 25, u64 n <= 51, u128 n <= 101,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A079496: a(0)=a(1)=1; for n≥1,
   a(2n)=4*a(2n-1)-a(2n-2);
   a(2n+1)=2*a(2n)-a(2n-1).
   Equivalently, a(n)=6*a(n-2)-a(n-4) for n≥4. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;         /* array of a[0 .. max_n] */
    static idx_t max_n = 0;             /* highest index stored */

    /* ensure at least two terms are initialized */
    if (max_n == 0) {
        cache = (seq_t *)realloc(cache, 2 * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        max_n  = 1;
    }

    /* grow cache if needed and compute new entries */
    if (n > max_n) {
        idx_t new_size = n + 1;                     /* indices 0..n */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;

        for (idx_t i = max_n + 1; i <= n; ++i) {
            if ((i & 1U) == 0) {              /* even index */
                cache[i] = (seq_t)4 * cache[i - 1] - cache[i - 2];
            } else {                           /* odd index */
                cache[i] = (seq_t)2 * cache[i - 1] - cache[i - 2];
            }
        }
        max_n = n;
    }

    return cache[n];
}
