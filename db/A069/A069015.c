/*
 * A069015  a(n) = n! * 3^n * Sum_{i=1..n} 1/(i * 3^i).
 * https://oeis.org/A069015
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1) = 1 Recurrence: a(n) = 3*n*a(n-1) + (n-1)! for n ≥ 2 (This
 *            follows from the definition a(n) = n!·3^n·∑_{i=1}^n 1/(i·3^i) )
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* handle legitimate indices only */
    if (n < 1) return 0;

    /* static memoisation structures */
    static seq_t *cache = NULL;
    static idx_t  cache_cap = 0;   /* capacity of cache array (indices 0..cache_cap) */
    static idx_t  last_done = 0;   /* largest n for which cache[n] is valid */
    static seq_t  fac = 1;         /* holds (last_done-1)!  (used to compute next factorial) */

    /* initialise cache on first call */
    if (cache == NULL) {
        /* initial capacity: at least enough for the first requested n */
        cache_cap = (n > 1 ? n : 2);
        cache = (seq_t *)malloc((cache_cap + 1) * sizeof(seq_t));
        if (!cache) return 0;                /* allocation failure → return 0 */
        cache[1] = (seq_t)1;                 /* a(1) = 1 */
        last_done = 1;
        fac = (seq_t)1;                      /* (1-1)! = 0! = 1 */
    }

    /* enlarge cache if needed */
    if (n > cache_cap) {
        idx_t newcap = cache_cap ? cache_cap : 2;
        while (newcap < n) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, (newcap + 1) * sizeof(seq_t));
        if (!tmp) return 0;                 /* allocation failure → return 0 */
        cache = tmp;
        cache_cap = newcap;
    }

    /* fill in values up to n */
    for (idx_t i = last_done + 1; i <= n; ++i) {
        /* update factorial: fac becomes (i-1)! */
        fac *= (seq_t)(i - 1);

        /* recurrence: a(i) = 3*i*a(i-1) + fac */
        seq_t prod = 3 * (seq_t)i * cache[i - 1];
        cache[i] = prod + fac;
    }

    last_done = n;
    return cache[n];
}
