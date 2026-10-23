/*
 * A192970  Coefficient of x in the reduction by x^2 -> x+1 of the polynomial
 *          p(n,x) defined at Comments.
 * https://oeis.org/A192970
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 5*a(n-2) + a(n-3) + 2*a(n-4) - a(n-5), a(0)=0,
 *            a(1)=1, a(2)=3, a(3)=9, a(4)=21
 * Range:     u8 n <= 7, u16 n <= 18, u32 n <= 41, u64 n <= 88, u128 n <= 180,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;          /* array of computed a(i) */
    static size_t cache_size = 0;        /* number of valid entries (max i+1) */

    /* if already computed return directly */
    if ((size_t)n < cache_size) {
        return cache[(size_t)n];
    }

    /* grow cache to accommodate up to n */
    size_t new_size = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) abort();          /* allocation failure */
    cache = tmp;

    /* compute missing values */
    for (size_t i = cache_size; i <= (size_t)n; ++i) {
        switch (i) {
            case 0: cache[i] = 0; break;
            case 1: cache[i] = 1; break;
            case 2: cache[i] = 3; break;
            case 3: cache[i] = 9; break;
            case 4: cache[i] = 21; break;
            default:
                /* recurrence: a(i)=4*a(i-1)-5*a(i-2)+a(i-3)+2*a(i-4)-a(i-5) */
                seq_t t = 4 * cache[i - 1];
                t += cache[i - 3];
                t += 2 * cache[i - 4];
                t -= 5 * cache[i - 2];
                t -= cache[i - 5];
                cache[i] = t;
                break;
        }
    }

    cache_size = new_size;
    return cache[(size_t)n];
}
