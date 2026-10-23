/*
 * A125513  a(n) is the number of binary strings of length n such that no
 *          subsequence of length 5 or less contains 4 or more ones.
 * https://oeis.org/A125513
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+a(n-2)+a(n-4)+2*a(n-5)-a(n-7)-a(n-10) with a(1)=2,
 *            a(2)=4, a(3)=8, a(4)=15, a(5)=26, a(6)=48, a(7)=89, a(8)=165,
 *            a(9)=305, a(10)=561
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>       /* for uint64_t */

seq_t a(idx_t n)
{
    /* Behavior for n < 1 is irrelevant; return 0. */
    if (n == 0) return (seq_t)0;

    /* Initial 10 terms stored as 64‑bit unsigned constants. */
    static const uint64_t init_uint[11] = {
        0ULL,   /* a(0) unused */
        2ULL,   /* a(1) */
        4ULL,   /* a(2) */
        8ULL,   /* a(3) */
        15ULL,  /* a(4) */
        26ULL,  /* a(5) */
        48ULL,  /* a(6) */
        89ULL,  /* a(7) */
        165ULL, /* a(8) */
        305ULL, /* a(9) */
        561ULL  /* a(10) */
    };

    /* Static cache for memoisation. */
    static seq_t *cache = NULL;     /* array of pre‑computed values */
    static size_t cache_cap = 0;    /* allocated size (>=1 for n==0) */
    static idx_t  cache_max = 0;    /* largest index stored */

    /* Ensure the cache can hold indices up to n. */
    size_t needed = (size_t)n + 1;  /* include index 0 so that cache[n] is valid */
    if (needed > cache_cap) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) abort();          /* out‑of‑memory */
        cache = tmp;
        cache_cap = needed;
    }

    /* Fill new entries up to n, using the recurrence where possible. */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        if (i <= 10) {
            /* Base cases – cast the 64‑bit constants to seq_t. */
            cache[i] = (seq_t) init_uint[i];
        } else {
            /* Recursive definition for i >= 11. */
            seq_t val =
                cache[i-1] + cache[i-2] + cache[i-4]
                + (cache[i-5] << 1)             /* 2 * a(i-5) */
                - cache[i-7] - cache[i-10];
            cache[i] = val;
        }
    }
    cache_max = n;

    return cache[n];
}
