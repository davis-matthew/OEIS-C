/*
 * A190444  E.g.f. exp(x+x^2+x^4).
 * https://oeis.org/A190444
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 2*(n-1)*a(n-2) + 4*(n-3)*(n-2)*(n-1)*a(n-4),
 *            initial values: a(0)=1, a(1)=1, a(2)=3, a(3)=7
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 13, u64 n <= 21, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;          /* memoization storage   */
    static size_t cap   = 0;             /* allocated size        */
    static idx_t  max_n = (idx_t)-1;     /* highest computed index */

    /* Ensure cache is initialized and large enough. */
    if (max_n == (idx_t)-1) {
        cap   = 4;                      /* need at least 4 entries  */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache[0] = 1;
        cache[1] = 1;
        cache[2] = 3;
        cache[3] = 7;
        max_n = 3;
    }

    if ((size_t)n >= cap) {
        size_t new_cap = cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cap   = new_cap;
    }

    for (idx_t i = max_n + 1; i <= n; ++i) {
        seq_t val;
        if (i == 0)           val = 1;
        else if (i == 1)       val = 1;
        else if (i == 2)       val = 3;
        else if (i == 3)       val = 7;
        else {
            seq_t a1 = cache[i-1];
            seq_t a2 = cache[i-2];
            seq_t a4 = cache[i-4];

            seq_t m1 = (seq_t)(i-1);
            seq_t m2 = (seq_t)(i-2);
            seq_t m3 = (seq_t)(i-3);

            seq_t term2 = 2 * m1 * a2;
            seq_t term4 = 4 * m3 * m2 * m1 * a4;

            val = a1 + term2 + term4;
        }
        cache[i] = val;
    }

    max_n = n;
    return cache[n];
}
