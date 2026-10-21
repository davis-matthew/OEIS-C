/*
 * A226348  Number of ways to tile a fixed 3 x n square grid with 1 x 1, 2 x
 *          2, and axially-oriented 1 x 2 tiles.
 * https://oeis.org/A226348
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1)+15*a(n-2)-a(n-3)-26*a(n-4)+5*a(n-5)+6*a(n-6) for
 *            n>6; a(1) = 1, a(2) = 12, a(3) = 39, a(4) = 233, a(5) = 1018,
 *            a(6) = 5191
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* cache[i] stores a(i+1)   */
static idx_t cache_limit = 0;    /* number of elements cached */

/*
 * Compute the nth term of A226348.
 * Uses memoisation with a static cache; works for any order of calls.
 */
seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* undefined for n < 1 */

    /* Extend cache if necessary */
    if (n > cache_limit) {
        idx_t new_limit = n;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_limit);
        if (!tmp)
            exit(EXIT_FAILURE);   /* allocation failed */
        cache = tmp;

        /* Initialize newly allocated entries */
        for (idx_t i = cache_limit + 1; i <= new_limit; ++i) {
            if (i == 1)
                cache[i - 1] = (seq_t)1;
            else if (i == 2)
                cache[i - 1] = (seq_t)12;
            else if (i == 3)
                cache[i - 1] = (seq_t)39;
            else if (i == 4)
                cache[i - 1] = (seq_t)233;
            else if (i == 5)
                cache[i - 1] = (seq_t)1018;
            else if (i == 6)
                cache[i - 1] = (seq_t)5191;
            else {
                seq_t v = 0;
                v += (seq_t)2   * cache[i - 2];   /* a(i-1) */
                v += (seq_t)15  * cache[i - 3];   /* a(i-2) */
                v -=            cache[i - 4];      /* a(i-3) */
                v -= (seq_t)26  * cache[i - 5];   /* a(i-4) */
                v += (seq_t)5   * cache[i - 6];   /* a(i-5) */
                v += (seq_t)6   * cache[i - 7];   /* a(i-6) */
                cache[i - 1] = v;
            }
        }
        cache_limit = new_limit;
    }

    return cache[n - 1];
}
