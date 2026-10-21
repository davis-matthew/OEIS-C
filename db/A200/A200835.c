/*
 * A200835  Number of 0..5 arrays x(0..n+1) of n+2 elements without any two
 *          consecutive increases or two consecutive decreases.
 * https://oeis.org/A200835
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 0..5 arrays x(0..n+1) of n+2 elements without *
 *            any two consecutive increases or two consecutive decreases
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 11, u64 n <= 25, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cache holds a(0), a(1), … up to the largest n computed so far */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    /* DP state for pairs (a,b) encoded as 6*a + b */
    static seq_t dp[36];
    static seq_t dp_next[36];
    /* current number of steps (i.e., length-2) already computed */
    static idx_t current_n = 0;

    /* Initialization: first call */
    if (current_n == 0) {
        /* start with length = 2, all 36 ordered pairs possible */
        for (idx_t s = 0; s < 36; ++s)
            dp[s] = (seq_t)1;

        /* cache[0] is unused; cache[1] will hold a(1) after first step */
        cache_cap = 2;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)
            return (seq_t)0;          /* allocation failure – unlikely in tests */
        cache[0] = (seq_t)0;
    }

    /* Return cached value if already computed */
    if (n <= current_n)
        return cache[n];

    /* Incrementally extend the DP to reach n */
    for (idx_t step = current_n + 1; step <= n; ++step) {
        /* compute next DP array */
        for (idx_t s = 0; s < 36; ++s)
            dp_next[s] = (seq_t)0;

        for (idx_t a = 0; a < 6; ++a) {
            for (idx_t b = 0; b < 6; ++b) {
                seq_t val = dp[a * 6 + b];
                if (val == 0)
                    continue;
                for (idx_t c = 0; c < 6; ++c) {
                    /* forbid two strict increases or two strict decreases */
                    if ((a < b && b < c) || (a > b && b > c))
                        continue;
                    dp_next[b * 6 + c] += val;
                }
            }
        }

        /* copy new DP state back */
        for (idx_t s = 0; s < 36; ++s)
            dp[s] = dp_next[s];

        /* sum of all states gives a(step) */
        seq_t sum = (seq_t)0;
        for (idx_t s = 0; s < 36; ++s)
            sum += dp[s];

        /* enlarge cache if necessary */
        if ((size_t)step >= cache_cap) {
            size_t new_cap = cache_cap * 2;
            if ((size_t)step >= new_cap)
                new_cap = (size_t)step + 1;
            seq_t *tmp = (seq_t *)realloc(cache,
                                           new_cap * sizeof(seq_t));
            if (!tmp)
                return sum;           /* allocation failure – unlikely */
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[step] = sum;
    }

    current_n = n;
    return cache[n];
}
