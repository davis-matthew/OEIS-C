/*
 * A168081  Lucas sequence U_n(x,1) over the field GF(2)[x].
 * https://oeis.org/A168081
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (2*a(n-1)) XOR a(n-2) (a(0)=0, a(1)=1)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache – grows as needed */
    static seq_t *cache = NULL;        /* array of computed terms */
    static size_t  cap   = 0;          /* current allocated capacity (elements) */
    static idx_t   len   = 0;          /* number of values stored so far */

    /* initialise base cases if cache is empty */
    if (cap == 0)
    {
        cap = 2;                       /* will hold a(0) and a(1) */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;          /* out of memory – unlikely in tests */
        cache[0] = 0;
        cache[1] = 1;
        len = 2;
    }

    /* ensure the cache has enough space for a(n) */
    if (n + 1 > cap)
    {
        size_t new_cap = cap;
        while (new_cap < (size_t)(n + 1))
            new_cap = new_cap * 2 + 1;  /* avoid overflow on small caps */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;           /* out of memory – unlikely */
        cache = tmp;
        cap = new_cap;
    }

    /* compute terms until we reach index n */
    while (len <= n)
    {
        /* compute a(len) = (2*a(len-1)) XOR a(len-2) */
        seq_t twice = cache[len - 1] + cache[len - 1]; /* 2*a(len-1) */
        cache[len] = twice ^ cache[len - 2];
        len++;
    }

    return cache[n];
}
