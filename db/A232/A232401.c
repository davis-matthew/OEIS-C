/*
 * A232401  Number of (n+1)X(3+1) 0..2 arrays with every element next to
 *          itself plus and minus one within the range 0..2 horizontally or
 *          vertically, with no adjacent elements equal.
 * https://oeis.org/A232401
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Empirical recurrence a(n) = 2*a(n-1) + 4*a(n-2) + a(n-3) -
 *            10*a(n-4) - 9*a(n-5) + 16*a(n-7) + 4*a(n-8) - a(n-9) - 4*a(n-10)
 *            - a(n-11) - a(n-12) Initial values: a(1) = 8, a(2) = 20, a(3) =
 *            52, a(4) = 176, a(5) = 484, a(6) = 1500, a(7) = 4416, a(8) =
 *            13220, a(9) = 39524, a(10) = 117892, a(11) = 352...
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* initialise cache with the first 12 terms */
    if (!cache) {
        cache_len = 12;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        cache[0] = (seq_t)8;
        cache[1] = (seq_t)20;
        cache[2] = (seq_t)52;
        cache[3] = (seq_t)176;
        cache[4] = (seq_t)484;
        cache[5] = (seq_t)1500;
        cache[6] = (seq_t)4416;
        cache[7] = (seq_t)13220;
        cache[8] = (seq_t)39524;
        cache[9] = (seq_t)117892;
        cache[10] = (seq_t)352408;
        cache[11] = (seq_t)1052272;
    }

    /* return value if already computed */
    if (n <= cache_len)
        return cache[n - 1];

    /* extend cache up to n */
    size_t new_len = n;
    cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));

    for (size_t i = cache_len; i < new_len; ++i) {
        /* i corresponds to index n-1 */
        /* all indices are valid because i >= 12 for the recurrence */
        seq_t pos = cache[i - 1] * (seq_t)2
                  + cache[i - 2] * (seq_t)4
                  + cache[i - 3]
                  + cache[i - 7] * (seq_t)16
                  + cache[i - 8] * (seq_t)4;

        seq_t neg = cache[i - 4] * (seq_t)10
                  + cache[i - 5] * (seq_t)9
                  + cache[i - 9]
                  + cache[i - 10] * (seq_t)4
                  + cache[i - 11]
                  + cache[i - 12];

        cache[i] = pos - neg;  /* pos >= neg for all n, result is non‑negative */
    }

    cache_len = new_len;
    return cache[n - 1];
}
