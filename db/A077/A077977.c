/*
 * A077977  Expansion of 1/(1+x+2*x^2-2*x^3).
 * https://oeis.org/A077977
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0)=1, a(1)=-1, a(2)=-1, for n>=3 : a(n) = -a(n-1) - 2*a(n-2) +
 *            2*a(n-3)
 * Range:     i8 n <= 8, i16 n <= 19, i32 n <= 39, i64 n <= 78, i128 n <= 156,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t sz = 0, cap = 0;

    if (n < sz)
        return cache[(size_t)n];

    /* enlarge cache if needed */
    size_t need = (size_t)n + 1;
    if (need > cap) {
        size_t newcap = cap ? cap << 1 : 16;
        while (newcap < need)
            newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) /* allocation failed – behaviour undefined */
            return (seq_t)0;
        cache = tmp;
        cap = newcap;
    }

    /* compute missing terms */
    for (size_t i = sz; i <= n; ++i) {
        if (i == 0)
            cache[i] = (seq_t)1;
        else if (i == 1)
            cache[i] = (seq_t)-1;
        else if (i == 2)
            cache[i] = (seq_t)-1;
        else
            cache[i] = -cache[i-1] - (seq_t)2 * cache[i-2] + (seq_t)2 * cache[i-3];
    }
    sz = need;
    return cache[(size_t)n];
}
