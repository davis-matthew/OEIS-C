/*
 * A068587  Multiplicative 3-wave sequence starting with 1,2,3.
 * https://oeis.org/A068587
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=2, a(2)=3, a(n)=a(n-1)*a(n-2) if n is odd,
 *            a(n)=a(n-1)*a(n-4) if n is even
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 9, u64 n <= 11, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;   /* cache of computed terms   */
    static size_t  cap     = 0;     /* current capacity of cache */
    static idx_t   maxcmp  = (idx_t)-1; /* highest index computed */

    /* initialise cache on first call */
    if (cache == NULL) {
        size_t init_cap = 8;
        cache = (seq_t*)malloc(init_cap * sizeof(seq_t));
        if (cache == NULL) abort();           /* allocation failure */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)3;
        cap = init_cap;
        maxcmp = 2;
    }

    if (n <= maxcmp)                         /* already known */
        return cache[n];

    /* enlarge cache if necessary */
    if ((size_t)(n + 1) > cap) {
        size_t new_cap = cap;
        while (new_cap <= (size_t)n) {
            new_cap = new_cap ? new_cap * 2 : 8;
        }
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();
        cache = tmp;
        cap = new_cap;
    }

    /* compute terms up to n */
    for (idx_t i = maxcmp + 1; i <= n; ++i) {
        seq_t val;
        if (i == 0)                val = (seq_t)1;
        else if (i == 1)           val = (seq_t)2;
        else if (i == 2)           val = (seq_t)3;
        else if (i & 1u)           /* odd */
            val = cache[i-1] * cache[i-2];
        else                       /* even */
            val = cache[i-1] * cache[i-4];
        cache[i] = val;
    }
    maxcmp = n;
    return cache[n];
}
