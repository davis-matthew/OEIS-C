/*
 * A221965  Number of -6..6 arrays of length n with the sum ahead of each
 *          element differing from the sum following that element by 6 or
 *          less.
 * https://oeis.org/A221965
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) + 18*a(n-2) - 56*a(n-3) - 48*a(n-4) + 112*a(n-5)
 *            + 32*a(n-6) - 64*a(n-7), a(1)=13, a(2)=169, a(3)=1105,
 *            a(4)=9633, a(5)=78273, a(6)=653185, a(7)=5407233
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_len = 7;                           /* store up to a(7) */
        cache = (seq_t *)malloc((cache_len + 1) * sizeof(seq_t));
        if (cache == NULL) exit(EXIT_FAILURE);

        cache[0] = (seq_t)0;                    /* unused */
        cache[1] = (seq_t)13;
        cache[2] = (seq_t)169;
        cache[3] = (seq_t)1105;
        cache[4] = (seq_t)9633;
        cache[5] = (seq_t)78273;
        cache[6] = (seq_t)653185;
        cache[7] = (seq_t)5407233;
    }

    if (n == 0) return (seq_t)0;                /* undefined, return 0 */

    if (n <= cache_len) return cache[n];

    /* extend cache up to n */
    idx_t new_len = n;
    seq_t *tmp = (seq_t *)realloc(cache, (new_len + 1) * sizeof(seq_t));
    if (tmp == NULL) exit(EXIT_FAILURE);
    cache = tmp;

    for (idx_t k = cache_len + 1; k <= new_len; ++k) {
        seq_t val = (seq_t)7 * cache[k - 1];
        val += (seq_t)18 * cache[k - 2];
        val -= (seq_t)56 * cache[k - 3];
        val -= (seq_t)48 * cache[k - 4];
        val += (seq_t)112 * cache[k - 5];
        val += (seq_t)32 * cache[k - 6];
        val -= (seq_t)64 * cache[k - 7];
        cache[k] = val;
    }
    cache_len = new_len;
    return cache[n];
}
