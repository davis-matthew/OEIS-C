/*
 * A069294  Number of n X 3 binary arrays with a path of adjacent 1's from
 *          upper left corner to anywhere in right hand column.
 * https://oeis.org/A069294
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*a(n-1) - 34*a(n-2) + 14*a(n-3) + 16*a(n-4) (n>=6)
 *            Initial values: a(2)=12, a(3)=110, a(4)=926, a(5)=7556
 *            (a(0)=a(1)=0 are unused but defined for safety)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;  /* number of elements stored (index 0..cache_len-1) */

    /* Initialise base table on first call */
    if (cache == NULL) {
        cache_len = 6;                     /* store indices 0..5 */
        cache = (seq_t *)realloc(NULL, cache_len * sizeof(seq_t));
        if (cache == NULL) return 0;       /* allocation failure returns 0 */
        cache[0] = 0;
        cache[1] = 0;
        cache[2] = (seq_t)12;
        cache[3] = (seq_t)110;
        cache[4] = (seq_t)926;
        cache[5] = (seq_t)7556;
    }

    if (n >= cache_len) {
        idx_t new_len = n + 1;            /* need indices up to n */
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (tmp == NULL) return 0;        /* allocation failure returns 0 */
        cache = tmp;
        /* Compute new entries using the recurrence */
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t val = 12 * cache[i-1];
            val -= 34 * cache[i-2];
            val += 14 * cache[i-3];
            val += 16 * cache[i-4];
            cache[i] = val;
        }
        cache_len = new_len;
    }

    /* Return the requested term (0 for indices <2 as they are unused) */
    return (n < cache_len) ? cache[n] : 0;
}
