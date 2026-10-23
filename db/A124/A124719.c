/*
 * A124719  Number of base 26 circular n-digit numbers with adjacent digits
 *          differing by 1 or less.
 * https://oeis.org/A124719
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum_{j=1..26} (1 + 2*cos(j*Pi/27))^n
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 19, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence counts the number of cyclic base‑26 words of length n
   where any two adjacent digits differ by at most 1.  */
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t cache_len = 0;           /* number of terms already cached (0..cache_len-1) */
    static seq_t dp_cur[26][26];
    static seq_t dp_next[26][26];
    static bool initialized = false;
    static idx_t current_len = 0;          /* length of dp_cur (number of steps) */

    if (!initialized) {
        /* allocate cache for a(0) and a(1) */
        cache_cap = 8;
        cache      = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0]   = (seq_t)1;                    /* a(0) = 1 */

        /* dp_cur holds walks of length 1 (adjacency matrix) */
        for (int i = 0; i < 26; ++i) {
            for (int j = 0; j < 26; ++j) {
                if (i == j || i + 1 == j || i == j + 1)
                    dp_cur[i][j] = (seq_t)1;
                else
                    dp_cur[i][j] = (seq_t)0;
            }
        }
        /* compute a(1) */
        seq_t a1 = 0;
        for (int i = 0; i < 26; ++i)
            a1 += dp_cur[i][i];
        cache[1]   = a1;      /* a(1) = 26 */
        cache_len = 2;        /* we have a(0), a(1) */
        current_len = 1;
        initialized = true;
    }

    if (n < cache_len)
        return cache[n];

    /* extend the cache up to n */
    for (idx_t len = current_len + 1; len <= n; ++len) {
        /* compute dp_next = dp_cur * adjacency matrix */
        for (int i = 0; i < 26; ++i) {
            for (int j = 0; j < 26; ++j) {
                seq_t val = dp_cur[i][j];
                if (j > 0)    val += dp_cur[i][j - 1];
                if (j < 25)   val += dp_cur[i][j + 1];
                dp_next[i][j] = val;
            }
        }

        /* sum of diagonal gives a(len) */
        seq_t sum = 0;
        for (int i = 0; i < 26; ++i)
            sum += dp_next[i][i];

        /* grow cache if necessary */
        if (cache_len >= cache_cap) {
            idx_t new_cap = cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp)   /* allocation failed: return 0 as a sentinel */
                return (seq_t)0;
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[cache_len] = sum;
        ++cache_len;

        /* prepare dp_cur for next iteration */
        for (int i = 0; i < 26; ++i)
            for (int j = 0; j < 26; ++j)
                dp_cur[i][j] = dp_next[i][j];
    }

    current_len = n;
    return cache[n];
}
