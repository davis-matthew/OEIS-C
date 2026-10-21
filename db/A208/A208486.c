/*
 * A208486  Number of (n+1) X 3 0..3 arrays with the number of clockwise edge
 *          increases in every 2 X 2 subblock equal to the number of
 *          counterclockwise edge increases.
 * https://oeis.org/A208486
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 32*a(n-1) - 227*a(n-2) + 528*a(n-3) - 328*a(n-4) -
 *            104*a(n-5) Initial terms (offset = 1): a(1) = 1448 a(2) = 33416
 *            a(3) = 773288 a(4) = 17903016 a(5) = 414522184
 * Range:     u8 none, u16 n <= 2, u32 n <= 5, u64 n <= 12, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static memoization array, 1-based indexing (cache[0] = a(1)) */
    static seq_t *cache = NULL;
    static idx_t cache_sz = 0;   /* number of terms currently stored */

    if (n == 0)  /* undefined, but return 0 for safety */
        return 0;

    /* resize the cache if necessary */
    if (cache == NULL || cache_sz < n)
    {
        idx_t new_sz = n;
        seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_sz);
        if (new_cache == NULL)
            exit(1);          /* allocation failed */
        cache = new_cache;

        /* compute new terms up to n */
        for (idx_t i = cache_sz + 1; i <= n; ++i)
        {
            seq_t val;
            if (i == 1)       val = 1448;
            else if (i == 2)  val = 33416;
            else if (i == 3)  val = 773288;
            else if (i == 4)  val = 17903016;
            else if (i == 5)  val = 414522184;
            else
            {
                /* use the recurrence */
                seq_t pos = ((seq_t)32) * cache[i - 2] + ((seq_t)528) * cache[i - 4];
                seq_t neg = ((seq_t)227) * cache[i - 3]
                          + ((seq_t)328) * cache[i - 5]
                          + ((seq_t)104) * cache[i - 6];
                val = pos - neg;          /* holds for all i ≥ 6 */
            }
            cache[i - 1] = val;
        }
        cache_sz = n;
    }

    return cache[n - 1];
}
