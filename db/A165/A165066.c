/*
 * A165066  Number of n-digit fixed points under the base-6 Kaprekar map
 *          A165051.
 * https://oeis.org/A165066
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = -a(n-1) + a(n-3) + 2*a(n-4) + 2*a(n-5) + a(n-6) * -
 *            a(n-7) - 2*a(n-8) - 2*a(n-9) - a(n-10) * + a(n-12) + a(n-13) (n
 *            > 15) * Initial values: a(1)=1, a(2)=0, a(3)=1, a(4)=0, a(5)=1,
 *            a(6)=3, * a(7)=0, a(8)=2, a(9)=1, a(10)=4, a(11)=2, a(12)=3, *
 *            a(13)=2, a(14)=5, a(15)=4
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation across any call order */
    static seq_t *cache = NULL;
    static idx_t cached = 0;         /* number of elements reserved (indices 0..cached-1) */

    if (n == 0) return 0;            /* safety: OEIS offset starts at 1 */

    /* Ensure the cache can hold up to index n (0..n) */
    if (n + 1 > cached) {
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * (n + 1));
        if (!tmp) return 0;          /* allocation failed */
        cache = tmp;
        cached = n + 1;
    }

    /* Initial values (indices 1‑15). cache[0] unused. */
    static const seq_t init[16] = {
        0ULL, /* 0 unused */
        1ULL, /* a(1) */
        0ULL, /* a(2) */
        1ULL, /* a(3) */
        0ULL, /* a(4) */
        1ULL, /* a(5) */
        3ULL, /* a(6) */
        0ULL, /* a(7) */
        2ULL, /* a(8) */
        1ULL, /* a(9) */
        4ULL, /* a(10) */
        2ULL, /* a(11) */
        3ULL, /* a(12) */
        2ULL, /* a(13) */
        5ULL, /* a(14) */
        4ULL  /* a(15) */
    };

    /* Compute all missing values up to n */
    for (idx_t i = 1; i <= n; ++i)
    {
        if (i <= 15)
        {
            cache[i] = init[i];
        }
        else
        {
            /* Build according to the recurrence */
            seq_t val = cache[i - 3];          /* + a(n-3) */
            val += cache[i - 4] + cache[i - 4];/* + 2*a(n-4) */
            val += cache[i - 5] + cache[i - 5];/* + 2*a(n-5) */
            val += cache[i - 6];               /* + a(n-6) */
            val += cache[i - 12];              /* + a(n-12) */
            val += cache[i - 13];              /* + a(n-13) */
            val -= cache[i - 1];               /* - a(n-1) */
            val -= cache[i - 7];               /* - a(n-7) */
            val -= cache[i - 8] + cache[i - 8];/* - 2*a(n-8) */
            val -= cache[i - 9] + cache[i - 9];/* - 2*a(n-9) */
            val -= cache[i - 10];              /* - a(n-10) */
            cache[i] = val;
        }
    }

    return cache[n];
}
