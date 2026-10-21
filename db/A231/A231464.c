/*
 * A231464  Number of (1+1) X (n+1) 0..3 arrays with no element unequal to a
 *          strict majority of its horizontal, diagonal and antidiagonal
 *          neighbors, with values 0..3 introduced in row major order.
 * https://oeis.org/A231464
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) + 3*a(n-2) - 41*a(n-3) + 11*a(n-4) + 137*a(n-5)
 *            - 82*a(n-6) - 176*a(n-7) + 108*a(n-8) + 108*a(n-9) - 72*a(n-10)
 *            Initial values: a(1)=3, a(2)=7, a(3)=14, a(4)=33, a(5)=78,
 *            a(6)=197, a(7)=531, a(8)=1471, a(9)=4350, a(10)=13011 The
 *            routine memoizes computed terms in a static array so...
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 20, u64 n <= 39, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Sequence indices start at 1 – behaviour for n==0 is irrelevant. */
    if (n == 0) return 0;

    /* First ten values of the sequence. */
    static const seq_t init[10] = {
        3, 7, 14, 33, 78, 197, 531, 1471, 4350, 13011
    };

    /* Static cache of previously computed values. */
    static seq_t *cache = NULL;      /* cache[0] unused, indices start at 1 */
    static idx_t  cache_cap = 0;     /* allocated size in terms of indices */
    static idx_t  last_used = 0;     /* highest index that has been computed */

    /* Allocate / resize cache if necessary. */
    if (cache_cap <= n) {
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp)          /* unlikely to happen in the test harness */
            return 0;
        cache = tmp;
        cache_cap = n + 1;
    }

    /* Initialise the cache with the first ten predetermined values. */
    if (last_used < 10) {
        idx_t i;
        for (i = last_used + 1; i <= 10 && i <= n; ++i)
            cache[i] = init[i - 1];
        last_used = (last_used < 10) ? 10 : last_used;
    }

    /* Compute missing terms up to n using the linear recurrence. */
    for (idx_t i = last_used + 1; i <= n; ++i) {
        /* All arithmetic stays in seq_t.  For built‑in unsigned types
           overflow is modulo 2^BIT, which is fine because we only
           validate terms that fit in the type. */
        cache[i] =  5 * cache[i - 1]
                  + 3 * cache[i - 2]
                  - 41 * cache[i - 3]
                  + 11 * cache[i - 4]
                  + 137 * cache[i - 5]
                  - 82 * cache[i - 6]
                  - 176 * cache[i - 7]
                  + 108 * cache[i - 8]
                  + 108 * cache[i - 9]
                  - 72 * cache[i - 10];
    }

    last_used = n;
    return cache[n];
}
