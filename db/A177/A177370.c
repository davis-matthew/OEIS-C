/*
 * A177370  (1+7*x-6*x^2)/(1-4*x-8*x^2+6*x^3).
 * https://oeis.org/A177370
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1)+8*a(n-2)-6*a(n-3) (n ≥ 4) Initial values:
 *            a(1)=1, a(2)=11, a(3)=46
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cache[0] holds a(1), cache[1] holds a(2), … */
    static idx_t cap   = 0;       /* allocated capacity (number of terms)        */
    static idx_t size  = 0;       /* largest index computed so far (≥0)          */

    if (n == 0) return 0; /* out of range – should not happen with normal use */

    if (n <= size) return cache[n - 1];  /* already computed */

    /* grow the array if necessary */
    idx_t needed = n;
    if (cap < needed) {
        idx_t newcap = (cap == 0) ? 8 : cap * 2;
        while (newcap < needed) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        cache = tmp;
        cap   = newcap;
    }

    /* compute missing terms */
    for (idx_t i = size + 1; i <= n; ++i) {
        if (i == 1)       cache[i - 1] = (seq_t)1;
        else if (i == 2)  cache[i - 1] = (seq_t)11;
        else if (i == 3)  cache[i - 1] = (seq_t)46;
        else {
            /* a(i) = 4*a(i-1) + 8*a(i-2) - 6*a(i-3) */
            seq_t term = cache[i - 2] * (seq_t)4
                       + cache[i - 3] * (seq_t)8
                       - cache[i - 4] * (seq_t)6;
            cache[i - 1] = term;
        }
    }

    size = n;
    return cache[n - 1];
}
