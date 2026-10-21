/*
 * A094789  Number of (s(0), s(1), ..., s(2n+1)) such that 0 < s(i) < 7 and
 *          |s(i) - s(i-1)| = 1 for i = 1,2,...,2n+1, s(0) = 1, s(2n+1) = 4.
 * https://oeis.org/A094789
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 6*a(n-2) + a(n-3) with initial values a(1)=1,
 *            a(2)=4, a(3)=14 The implementation keeps a memoised table and
 *            grows it as needed
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 38, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static memoised table: cache[0] corresponds to a(1) */
    static seq_t *cache = NULL;
    static idx_t cached_len = 0;   /* number of values currently stored */
    static idx_t capacity = 0;     /* allocated capacity of cache */

    /* ensure at least three elements for recurrence */
    if (cached_len < 3) {
        idx_t init_cap = 16;
        cache = (seq_t *)realloc(cache, init_cap * sizeof(seq_t));
        capacity = init_cap;
        /* initialize a(1), a(2), a(3) */
        cache[0] = (seq_t)1;   /* a(1) */
        cache[1] = (seq_t)4;   /* a(2) */
        cache[2] = (seq_t)14;  /* a(3) */
        cached_len = 3;
    }

    /* grow cache if necessary */
    if (n > cached_len) {
        idx_t needed = n;
        if (capacity < needed) {
            /* double capacity until it fits */
            while (capacity < needed)
                capacity <<= 1;
            cache = (seq_t *)realloc(cache, capacity * sizeof(seq_t));
        }

        /* compute missing terms up to n */
        for (idx_t i = cached_len + 1; i <= n; ++i) {
            /* i is 1-based index for a(i) */
            /* a(i-1) -> cache[i-2], a(i-2) -> cache[i-3], a(i-3) -> cache[i-4] */
            seq_t t1 = cache[i - 2]; /* a(i-1) */
            seq_t t2 = cache[i - 3]; /* a(i-2) */
            seq_t t3 = cache[i - 4]; /* a(i-3) */
            seq_t val = 5 * t1;
            val += t3;
            val -= 6 * t2;           /* val = 5*a(i-1) - 6*a(i-2) + a(i-3) */
            cache[i - 1] = val;
        }
        cached_len = n;
    }

    return cache[n - 1];   /* a(n) */
}
