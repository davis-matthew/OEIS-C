/*
 * A165782  Number of reduced words of length n in Coxeter group on 7
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^10 = I.
 * https://oeis.org/A165782
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Number of reduced words of length n in the Coxeter group on 7
 *            generators with relations S_i^2 = (S_iS_j)^10 = I. a(0) = 1,
 *            a(1) = 7, a(2) = 42, a(3) = 252, a(4) = 1512, a(5) = 9072, a(6)
 *            = 54432, a(7) = 326592, a(8) = 1959552, a(9) = 11757312. For n ≥
 *            10 the terms satisfy the linear recurrence a(...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to store already computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;  /* number of terms currently stored */

    /* initialise cache with the first ten values (a(0) … a(9)) */
    if (cache == NULL) {
        cache_len = 10;
        cache = (seq_t *)malloc((size_t)cache_len * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;  /* allocation failure */
        cache[0] = 1;
        cache[1] = 7;
        cache[2] = 42;
        cache[3] = 252;
        cache[4] = 1512;
        cache[5] = 9072;
        cache[6] = 54432;
        cache[7] = 326592;
        cache[8] = 1959552;
        cache[9] = 11757312;
    }

    /* if the requested term is already in the cache, return it */
    if (n < cache_len) return cache[n];

    /* extend the cache to include terms up to index n */
    size_t new_len = (size_t)(n + 1);
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (tmp == NULL) return (seq_t)0;          /* allocation failure */
    cache = tmp;

    /* compute missing terms using the recurrence */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t sum = 0;
        /* add 5 * a(i-j) for j = 1 .. 9 */
        for (int j = 1; j <= 9; ++j) {
            sum += (seq_t)5 * cache[i - j];
        }
        /* subtract 15 * a(i-10) */
        sum -= (seq_t)15 * cache[i - 10];
        /* add the numerator term for n = 10 (value = 1) */
        if (i == 10) sum += (seq_t)1;
        cache[i] = sum;
    }

    cache_len = n + 1;
    return cache[n];
}
