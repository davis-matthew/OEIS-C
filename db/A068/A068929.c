/*
 * A068929  Number of incongruent ways to tile a 4 X n room with 1 X 2 Tatami
 *          mats. At most 3 Tatami mats may meet at a point.
 * https://oeis.org/A068929
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for A068929:
   a(n) = a(n-3) + a(n-5) + a(n-6) - a(n-9) + a(n-10) - a(n-11) - a(n-13) - a(n-15)  for n >= 20
   Initial values:
   a(1)=1, a(2)=3, a(3)=2, a(4)=1, a(5)=2, a(6)=2, a(7)=2, a(8)=3, a(9)=3,
   a(10)=4, a(11)=5, a(12)=5, a(13)=6, a(14)=8, a(15)=8, a(16)=11, a(17)=12,
   a(18)=14, a(19)=17.
*/

#include <stdlib.h>

static const seq_t init[20] = {
    0ULL, /* unused, a(0) = 0 */
    1ULL, /* a(1) */
    3ULL, /* a(2) */
    2ULL, /* a(3) */
    1ULL, /* a(4) */
    2ULL, /* a(5) */
    2ULL, /* a(6) */
    2ULL, /* a(7) */
    3ULL, /* a(8) */
    3ULL, /* a(9) */
    4ULL, /* a(10) */
    5ULL, /* a(11) */
    5ULL, /* a(12) */
    6ULL, /* a(13) */
    8ULL, /* a(14) */
    8ULL, /* a(15) */
    11ULL,/* a(16) */
    12ULL,/* a(17) */
    14ULL,/* a(18) */
    17ULL /* a(19) */
};

seq_t a(idx_t n)
{
    /* Handle small n directly from the initial list. */
    if (n <= 19) {
        return init[n];
    }

    /* Static cache for memoisation. */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;  /* last computed index */

    /* Initialise the cache on first use. */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t) * 20);
        if (cache == NULL) return 0; /* Allocation failed – unlikely in tests. */
        for (idx_t i = 0; i < 20; ++i) {
            cache[i] = init[i];
        }
        cache_len = 19;
    }

    /* Grow the cache if necessary. */
    if (n > cache_len) {
        size_t new_len = (size_t)n + 1;  /* we need indices up to n */
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (tmp == NULL) return 0; /* Allocation failed */
        cache = tmp;
        /* Compute the missing values. */
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            /* Recurrence: a(i) = a(i-3) + a(i-5) + a(i-6)
                           - a(i-9) + a(i-10) - a(i-11) - a(i-13) - a(i-15) */
            seq_t val = 0;
            val += cache[i - 3];
            val += cache[i - 5];
            val += cache[i - 6];
            val -= cache[i - 9];
            val += cache[i - 10];
            val -= cache[i - 11];
            val -= cache[i - 13];
            val -= cache[i - 15];
            cache[i] = val;
        }
        cache_len = n;
    }

    return cache[n];
}
