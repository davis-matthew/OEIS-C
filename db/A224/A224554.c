/*
 * A224554  Number of (n+2) X 5 0..1 matrices with each 3 X 3 subblock
 *          idempotent.
 * https://oeis.org/A224554
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 6*a(n-2) + 5*a(n-3) - 4*a(n-4) + 3*a(n-5) -
 *            a(n-6) for n>8 Initial values: a(1)=113, a(2)=133, a(3)=180,
 *            a(4)=230, a(5)=320, a(6)=451, a(7)=620, a(8)=866
 * Range:     u8 n <= 4, u16 n <= 19, u32 n <= 48, u64 n <= 106, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache: cache[0] holds a(1) */
    static seq_t *cache = NULL;
    static size_t cap = 0;          /* allocated capacity of cache */
    static idx_t computed = 0;      /* number of terms that have been computed (at least 8) */

    /* Allocate or enlarge the cache if necessary.
       Cast the pointer returned by realloc to keep the code valid in C++. */
    if (cap < n || cap < 8) {
        size_t newcap = (cap == 0) ? 16 : cap << 1;
        while (newcap < n || newcap < 8) newcap <<= 1;
        cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        cap = newcap;
    }

    /* Initialise the first eight terms on the first call (or if the cache
       was just enlarged). The values are stored even if n<8, because
       subsequent calculations for n>8 require all of them. */
    if (computed == 0) {
        static const seq_t init[8] = {
            113ULL, 133ULL, 180ULL, 230ULL,
            320ULL, 451ULL, 620ULL, 866ULL
        };
        for (idx_t i = 0; i < 8; ++i) cache[i] = init[i];
        computed = 8;
    }

    /* If the requested term is already available, simply return it. */
    if (n <= computed)
        return cache[n - 1];

    /* Compute missing terms using the recurrence.
       The recurrence is valid for n>8, i.e., for k>=9. */
    for (idx_t k = 9; k <= n; ++k) {
        /* k is 1‑based: cache[k-1] = a(k) */
        seq_t val =
             4 * cache[k - 2]   /* a(k-1) */
           - 6 * cache[k - 3]   /* a(k-2) */
           + 5 * cache[k - 4]   /* a(k-3) */
           - 4 * cache[k - 5]   /* a(k-4) */
           + 3 * cache[k - 6]   /* a(k-5) */
           -   cache[k - 7];    /* a(k-6) */

        cache[k - 1] = val;
    }

    computed = n;
    return cache[n - 1];
}
