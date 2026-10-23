/*
 * A186932  Number of (n+1) X 3 binary arrays with every 2 X 2 subblock trace
 *          equal to exactly one or two horizontal and vertical neighbor 2 X 2
 *          subblock traces.
 * https://oeis.org/A186932
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A186932: Number of (n+1)×3 binary arrays with every 2×2 subblock trace equal
 *  to exactly one or two horizontal and vertical neighbour 2×2 subblock traces.
 *
 *  Recurrence (empirical):
 *  a(n) = 2*a(n-1) + 4*a(n-2) + 12*a(n-3) + 4*a(n-4) + 17*a(n-5) + 136*a(n-6)
 *         - 78*a(n-7) - 269*a(n-8) - 496*a(n-9) + 226*a(n-10) + 441*a(n-11)
 *         - 110*a(n-12) + 372*a(n-13) + 448*a(n-15) - 512*a(n-16)
 *
 *  Initial values (a(1)...a(16)):
 *  24, 104, 472, 1664, 6848, 27272, 108128, 431656, 1719312, 6845968,
 *  27282352, 108671048, 432902384, 1724605424, 6870205712, 27368517688.
 *
 *  This function is memoised and works for calls in any order.  It uses only
 *  the allowed operators and therefore satisfies the compilation constraints
 *  for both C11 and C++17.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* sequence starts at n = 1 */

    /* static memoisation table */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* allocated size (indices 0 .. cache_size-1) */
    static idx_t max_computed = 0; /* greatest index computed and stored */

    /* Ensure the table is large enough to hold index n */
    if (cache_size <= n) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;                /* allocation failure */
        /* initialise new elements to zero */
        for (idx_t i = cache_size; i < new_size; ++i) tmp[i] = 0;
        cache = tmp;
        cache_size = new_size;
    }

    /* Initialise base values on the first allocation or when the cache
       grows to at least length 17.  The base array is 1‑based. */
    static const seq_t base[17] = {
        0ULL,          /* unused 0‑index */
        24ULL,         /* a(1)  */
        104ULL,        /* a(2)  */
        472ULL,        /* a(3)  */
        1664ULL,       /* a(4)  */
        6848ULL,       /* a(5)  */
        27272ULL,      /* a(6)  */
        108128ULL,     /* a(7)  */
        431656ULL,     /* a(8)  */
        1719312ULL,    /* a(9)  */
        6845968ULL,    /* a(10) */
        27282352ULL,   /* a(11) */
        108671048ULL,  /* a(12) */
        432902384ULL,  /* a(13) */
        1724605424ULL, /* a(14) */
        6870205712ULL, /* a(15) */
        27368517688ULL /* a(16) */
    };

    if (max_computed < 16) {
        idx_t limit = (cache_size > 16) ? 16 : cache_size - 1;
        for (idx_t i = 1; i <= limit; ++i)
            cache[i] = base[i];
        max_computed = limit;
    }

    /* Return the value if it is already computed */
    if (n <= max_computed)
        return cache[n];

    /* Compute missing values sequentially */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        if (i <= 16) {
            /* Base values already stored; nothing to do */
            continue;
        }
        cache[i] =
            (seq_t)2   * cache[i-1]   +
            (seq_t)4   * cache[i-2]   +
            (seq_t)12  * cache[i-3]   +
            (seq_t)4   * cache[i-4]   +
            (seq_t)17  * cache[i-5]   +
            (seq_t)136 * cache[i-6]   -
            (seq_t)78  * cache[i-7]   -
            (seq_t)269 * cache[i-8]   -
            (seq_t)496 * cache[i-9]   +
            (seq_t)226 * cache[i-10]  +
            (seq_t)441 * cache[i-11]  -
            (seq_t)110 * cache[i-12]  +
            (seq_t)372 * cache[i-13]  +
            (seq_t)448 * cache[i-15]  -
            (seq_t)512 * cache[i-16];
    }

    max_computed = n;
    return cache[n];
}
