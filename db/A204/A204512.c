/*
 * A204512  Square roots of [A055872/8]: Their square written in base 8, with
 *          some digit appended, is again a square.
 * https://oeis.org/A204512
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1)=a(2)=a(3)=0, a(4)=1, a(5)=2, a(n)=6*a(n-2)-a(n-4) (n>=6)
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 29, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;  /* a(0) is undefined, but return 0 for safety */

    /* static cache for memoisation; indexed from 1 */
    static seq_t *cache = NULL;
    static idx_t cap   = 0;   /* allocated size */
    static idx_t last  = 0;   /* largest n computed so far */

    /* Grow cache if needed */
    if (cap <= n) {
        idx_t new_cap = cap ? cap << 1 : 16;
        while (new_cap <= n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            exit(EXIT_FAILURE);   /* cannot allocate */
        cache = tmp;
        cap = new_cap;
        /* No need to initialise new slots: they will be written before use */
    }

    /* Compute all missing terms up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        if (i == 1)
            cache[1] = (seq_t)0;
        else if (i == 2)
            cache[2] = (seq_t)0;
        else if (i == 3)
            cache[3] = (seq_t)0;
        else if (i == 4)
            cache[4] = (seq_t)1;
        else if (i == 5)
            cache[5] = (seq_t)2;
        else
            cache[i] = (seq_t)6 * cache[i - 2] - cache[i - 4];
    }
    last = n;
    return cache[n];
}
