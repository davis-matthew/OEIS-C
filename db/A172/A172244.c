/*
 * A172244  Integers (up to a sign) that are not representable in the form
 *          2x^2 + xy + 3y^2 + z^3 - z for integer x,y,z.
 * https://oeis.org/A172244
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 15550*a(n-2) - a(n-4) for n>4;
 * Initial values: a(1)=1, a(2)=599, a(3)=14951, a(4)=9314449
 */
#include <stdlib.h>   /* malloc, realloc */

seq_t a(idx_t n)
{
    /* a(0) is undefined; if called return 0 */
    if (n == 0) return (seq_t)0;

    /* 1‑based cache, index 0 unused */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;      /* allocated slots (max index + 1) */
    static size_t max_computed = 0;    /* highest index computed so far */

    /* extend cache if needed */
    if ((size_t)n > max_computed)
    {
        size_t new_size = (size_t)n + 1;          /* indices 0..n */
        seq_t *new_cache = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (new_cache == NULL)                    /* allocation failure */
            return (seq_t)0;
        cache = new_cache;
        cache_size = new_size;

        /* compute missing terms */
        for (size_t i = max_computed + 1; i <= (size_t)n; ++i)
        {
            if (i <= 4)            /* initial values */
            {
                switch (i)
                {
                case 1:  cache[i] = (seq_t)1;          break;
                case 2:  cache[i] = (seq_t)599;        break;
                case 3:  cache[i] = (seq_t)14951;      break;
                case 4:  cache[i] = (seq_t)9314449;    break;
                }
            }
            else                        /* recurrence */
            {
                cache[i] = (seq_t)15550 * cache[i - 2] - cache[i - 4];
            }
        }
        max_computed = (size_t)n;
    }

    return cache[n];
}
