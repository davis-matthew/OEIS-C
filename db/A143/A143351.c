/*
 * A143351  Expansion of x/(1 -x^2 -x^4 -x^7 -x^8 -x^9 -x^10).
 * https://oeis.org/A143351
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 37, u32 n <= 70, u64 n <= 135,
 *            u128 n <= 265, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A143351
   Recurrence: a(n) = a(n-2)+a(n-4)+a(n-7)+a(n-8)+a(n-9)+a(n-10)
   Initial values: a(1)=1, a(2)=0, a(3)=1, a(4)=0, a(5)=2, a(6)=0, a(7)=3,
                    a(8)=1, a(9)=6, a(10)=3
*/
#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for n=0, but safe */

    static seq_t *cache = NULL;
    static size_t   cap   = 0;
    static idx_t    max_c = 0;

    /* grow the cache if necessary */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = (cap < 16 ? 16 : cap * 2);
        if ((size_t)n + 1 > new_cap)
            new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            return 0; /* allocation failed */
        cache = tmp;
        cap   = new_cap;
    }

    /* initialise base terms once */
    if (max_c == 0) {
        if (cap < 11) {              /* need indices 0..10 */
            size_t new_cap = 64;     /* larger than minimal */
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp)
                return 0;
            cache = tmp;
            cap   = new_cap;
        }
        cache[0]  = 0;  /* dummy, keep 1‑based alignment */
        cache[1]  = 1;
        cache[2]  = 0;
        cache[3]  = 1;
        cache[4]  = 0;
        cache[5]  = 2;
        cache[6]  = 0;
        cache[7]  = 3;
        cache[8]  = 1;
        cache[9]  = 6;
        cache[10] = 3;
        max_c = 10;
    }

    /* compute any missing terms up to n */
    for (idx_t i = max_c + 1; i <= n; ++i) {
        seq_t sum = 0;
        if (i >= 2) sum += cache[i - 2];
        if (i >= 4) sum += cache[i - 4];
        if (i >= 7) sum += cache[i - 7];
        if (i >= 8) sum += cache[i - 8];
        if (i >= 9) sum += cache[i - 9];
        if (i >= 10) sum += cache[i - 10];
        cache[i] = sum;
    }
    if (n > max_c)
        max_c = n;

    return cache[n];
}
