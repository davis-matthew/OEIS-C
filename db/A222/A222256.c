/*
 * A222256  Lexicographically earliest injective sequence of nonnegative
 *          integers such that the sum of 6 consecutive terms is always
 *          divisible by 6.
 * https://oeis.org/A222256
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-3) - a(n-6) with base values: a(0) = 0, a(1) = 1,
 *            a(2) = 2, a(3) = 3, a(4) = 4, a(5) = 8
 * Range:     u8 n <= 130, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses memoisation with a static array that
   grows on demand.  It works for any idx_t (e.g. uint64_t) and any
   seq_t, including arbitrary‑precision types; it only uses arithmetic
   and pointer operations that are valid in both C11 and C++17. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation. */
    static seq_t *cache = NULL;   /* the array of terms 0 … (cache_len-1) */
    static size_t cache_len = 0;  /* how many terms are stored */

    /* If the value is already computed, return it immediately. */
    if ((size_t)n < cache_len)
        return cache[(size_t)n];

    /* Grow the cache to size n+1.  Under C++ realloc returns void*,
       so we cast it explicitly. */
    size_t new_len = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) {                       /* out of memory – abort */
        exit(EXIT_FAILURE);
    }
    cache = tmp;

    /* Compute the missing terms up to n. */
    for (size_t i = cache_len; i <= n; ++i) {
        if (i <= 5) {                  /* base cases */
            switch ((idx_t)i) {
                case 0: cache[i] = (seq_t)0; break;
                case 1: cache[i] = (seq_t)1; break;
                case 2: cache[i] = (seq_t)2; break;
                case 3: cache[i] = (seq_t)3; break;
                case 4: cache[i] = (seq_t)4; break;
                case 5: cache[i] = (seq_t)8; break;
            }
        } else {
            /* Recurrence: a(i) = 2*a(i-3) - a(i-6) */
            cache[i] = (seq_t)2 * cache[i - 3] - cache[i - 6];
        }
    }

    /* Update cache length and return the requested value. */
    cache_len = new_len;
    return cache[(size_t)n];
}
