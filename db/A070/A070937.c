/*
 * A070937  Number of times maximal coefficient (A025591) appears in
 *          Product_{k<=n} (x^k + 1), i.e., number of times highest value
 *          appears in n-th row of A053632 or n-th column of A070936.
 * https://oeis.org/A070937
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1,a(1)=2,a(2)=4,a(3)=1,a(4)=5,a(5)=6,a(6)=4,a(7)=5,a(8)=1,
 *            a(9)=4,a(10)=2,a(11)=1,a(12)=1; a(n)=a(n-1)-a(n-2)+a(n-3) for
 *            n>12
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* initial values for indices 0..12 */
    static const seq_t init_vals[13] = {
        1, /* a(0) */
        2, /* a(1) */
        4, /* a(2) */
        1, /* a(3) */
        5, /* a(4) */
        6, /* a(5) */
        4, /* a(6) */
        5, /* a(7) */
        1, /* a(8) */
        4, /* a(9) */
        2, /* a(10) */
        1, /* a(11) */
        1  /* a(12) */
    };

    /* ensure array large enough */
    if (cache_len == 0) {
        /* Allocate at least 13 entries initially */
        cache_len = 13;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_len);
        for (idx_t i = 0; i < 13; ++i)
            cache[i] = init_vals[i];
    }

    if (n >= cache_len) {
        /* extend cache to accommodate n */
        idx_t new_len = n + 1;
        if (new_len < cache_len * 2)
            new_len = cache_len * 2;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_len);
        if (!tmp) {
            /* allocation failed; best effort return 0 */
            return 0;
        }
        cache = tmp;
        /* compute new entries using recurrence */
        for (idx_t i = cache_len; i < new_len; ++i) {
            if (i <= 12)
                cache[i] = init_vals[i];
            else {
                long long val = (long long)cache[i-1]
                              - (long long)cache[i-2]
                              + (long long)cache[i-3];
                cache[i] = (seq_t)val;
            }
        }
        cache_len = new_len;
    }

    return cache[n];
}
