/*
 * A228477  Number of nX3 binary arrays with top left value 1 and no two ones
 *          adjacent horizontally, vertically or antidiagonally.
 * https://oeis.org/A228477
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) +5*a(n-2) +4*a(n-3) – a(n-5) Initial values:
 *            a(1)=2, a(2)=4, a(3)=14, a(4)=41, a(5)=127
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* fast return for no-call or for n=0 (undefined in OEIS) */
    if (n == (idx_t)0) return 0;

    static seq_t *cache = NULL;   /* indexed from 1 */
    static idx_t last  = 0;       /* largest n computed so far */
    static size_t cap  = 0;       /* allocated capacity (max index that can be stored) */

    /* initial allocation of first 5 terms */
    if (cap == 0) {
        cap = 5;
        cache = (seq_t *)malloc((cap + 1) * sizeof(seq_t));
        if (!cache) return 0;   /* out of memory; caller will handle */
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)14;
        cache[4] = (seq_t)41;
        cache[5] = (seq_t)127;
        last = 5;
    }

    /* if the desired n is already computed, return it immediately */
    if (n <= last) return cache[n];

    /* ensure we have enough space to write up to n */
    if ((size_t)n > cap) {
        size_t newcap = cap * 2;
        if (newcap < (size_t)n) newcap = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, (newcap + 1) * sizeof(seq_t));
        if (!tmp) return 0;   /* out of memory */
        cache = tmp;
        cap   = newcap;
    }

    /* compute values from last+1 up to n using the recurrence */
    for (idx_t i = last + 1; i <= n; ++i) {
        /* recurrence is valid for i >= 6; i-5 >= 1 because last starts at 5 */
        cache[i] = cache[i - 1]
                 + (seq_t)5 * cache[i - 2]
                 + (seq_t)4 * cache[i - 3]
                 -         cache[i - 5];
    }
    last = n;
    return cache[n];
}
