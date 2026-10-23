/*
 * A160212  Positive numbers y such that y^2 is of the form x^2+(x+953)^2 with
 *          integer x.
 * https://oeis.org/A160212
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=845,a(2)=953,a(3)=1093,a(4)=3977,a(5)=4765,a(6)=5713;
 *            a(n)=6*a(n-3)-a(n-6) for n>6
 * Range:     u8 none, u16 n <= 9, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>  /* malloc, realloc, exit */

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;       /* cache[0] = a(1) */
    static idx_t sz = 0;              /* number of cached elements */

    if (n == 0) return 0;  /* undefined for n<1, return 0 */

    /* expand cache if needed */
    if (n > sz)
    {
        idx_t new_sz = n;
        seq_t *tmp = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
        if (tmp == NULL) exit(EXIT_FAILURE);  /* allocation failed */
        cache = tmp;
        /* fill new entries */
        for (idx_t i = sz + 1; i <= n; ++i)
        {
            if (i == 1)      cache[i-1] = (seq_t)845;
            else if (i == 2) cache[i-1] = (seq_t)953;
            else if (i == 3) cache[i-1] = (seq_t)1093;
            else if (i == 4) cache[i-1] = (seq_t)3977;
            else if (i == 5) cache[i-1] = (seq_t)4765;
            else if (i == 6) cache[i-1] = (seq_t)5713;
            else
            {
                /* a(i) = 6*a(i-3) - a(i-6) */
                seq_t t = 6 * cache[i-4]; /* i-4 indexes a(i-3) */
                t -= cache[i-7];          /* i-7 indexes a(i-6) */
                cache[i-1] = t;
            }
        }
        sz = new_sz;
    }
    return cache[n-1];
}
