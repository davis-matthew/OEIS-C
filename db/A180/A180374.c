/*
 * A180374  1/120 the number of n X 3 arrays of integers in 1..6 with no
 *          element or any of its neighbors having the same value.
 * https://oeis.org/A180374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) + 18*a(n-2) - 32*a(n-3) for n ≥ 5; a(1)=1,
 *            a(2)=21, a(3)=156, a(4)=1236
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0; /* number of terms currently stored (n) */

    if (n < 1)
        return (seq_t)0; /* outside defined offset */

    /* Resize cache if needed */
    if (cache_size < n)
    {
        idx_t new_size = n;
        seq_t *new_cache = (seq_t *)realloc(cache, (size_t)new_size * sizeof(seq_t));
        if (!new_cache)
            return (seq_t)0;          /* allocation failure – fallback */

        /* Compute new entries, starting after old size */
        for (idx_t i = cache_size + 1; i <= n; ++i)
        {
            seq_t val;
            switch (i)
            {
                case 1: val = (seq_t)1;          break;
                case 2: val = (seq_t)21;         break;
                case 3: val = (seq_t)156;        break;
                case 4: val = (seq_t)1236;       break;
                default:
                    val = 6 * new_cache[i - 2]     /* a(i-1) */
                      + 18 * new_cache[i - 3]     /* a(i-2) */
                      - 32 * new_cache[i - 4];    /* a(i-3) */
                    break;
            }
            new_cache[i - 1] = val;
        }

        cache = new_cache;
        cache_size = new_size;
    }

    return cache[n - 1];
}
