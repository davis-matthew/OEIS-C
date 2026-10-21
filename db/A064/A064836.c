/*
 * A064836  a(n) = A064835(n)/2.
 * https://oeis.org/A064836
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) calculated from the rational generating function x^2*(1 + x
 *            + 7*x^2 - 3*x^3)/((1 - x)^5*(1 + x)); Recurrence with initial
 *            values a(0)=0,a(1)=0,a(2)=1,a(3)=5,a(4)=22,a(5)=60; for n>=6:
 *            a(n)=4*a(n-1)-5*a(n-2)+5*a(n-4)-4*a(n-5)+a(n-6)
 * Range:     u8 n <= 6, u16 n <= 26, u32 n <= 430, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* initial terms 0..5 */
    static const seq_t init[6] = {0, 0, 1, 5, 22, 60};

    /* cache for previously computed values */
    static seq_t *cache = NULL;
    static idx_t  cache_size = 0;

    /* if already computed, return */
    if (n < cache_size)
        return cache[n];

    /* enlarge cache to hold up to index n */
    idx_t new_size = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp)
        abort();                       /* allocation failed */
    cache = tmp;

    /* compute all new terms up to n */
    for (idx_t i = cache_size; i <= n; ++i)
    {
        if (i < 6) {                     /* initialise first 6 entries */
            cache[i] = init[i];
            continue;
        }

        seq_t val = cache[i - 1];
        val *= 4;

        seq_t t = cache[i - 2];
        t *= 5;
        val -= t;

        t = cache[i - 4];
        t *= 5;
        val += t;

        t = cache[i - 5];
        t *= 4;
        val -= t;

        val += cache[i - 6];

        /* add the inhomogeneous term b(i) */
        switch (i) {
            case 2: val += 1; break;
            case 3: val += 1; break;
            case 4: val += 7; break;
            case 5: val -= 3; break;
            default: break;
        }

        cache[i] = val;
    }

    cache_size = new_size;
    return cache[n];
}
