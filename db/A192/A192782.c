/*
 * A192782  Coefficient of x in the reduction of the n-th Fibonacci polynomial
 *          by x^3->x^2+1.
 * https://oeis.org/A192782
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1)+3*a(n-2)-a(n-3)-3*a(n-4)+a(n-5)+a(n-6)
   Initial values: a(1)=0, a(2)=0, a(3)=1, a(4)=1, a(5)=4, a(6)=6 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)    /* safety for index 0; not used in OEIS */
        return 0;

    static seq_t *cache = NULL;
    static idx_t sz = 0;            /* largest precomputed index */

    if (n <= sz)
        return cache[n];

    /* enlarge cache to at least n */
    idx_t new_sz = n;
    seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * (new_sz + 1));
    if (!tmp) abort();   /* allocation failure */
    cache = tmp;

    /* base values a(1)..a(6) */
    static const seq_t base[6] = {0, 0, 1, 1, 4, 6};
    if (sz < 6) {
        idx_t to = (new_sz < 6 ? new_sz : 6);
        for (idx_t i = sz + 1; i <= to; ++i)
            cache[i] = base[i - 1];
    }

    /* compute missing terms using the recurrence */
    for (idx_t k = sz + 1; k <= new_sz; ++k) {
        if (k <= 6) continue;       /* already filled */
        seq_t v1 = cache[k - 1];
        seq_t v2 = cache[k - 2];
        seq_t v3 = cache[k - 3];
        seq_t v4 = cache[k - 4];
        seq_t v5 = cache[k - 5];
        seq_t v6 = cache[k - 6];
        seq_t pos = v1 + 3 * v2;
        pos += v5 + v6;
        seq_t neg = v3 + 3 * v4;
        cache[k] = pos - neg;
    }

    sz = new_sz;
    return cache[n];
}
