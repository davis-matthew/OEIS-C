/*
 * A121953  a(n) = (n-2)*a(n-2) + a(n-3), with a(1)=0, a(2)=1, a(3)=1.
 * https://oeis.org/A121953
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-2)*a(n-2) + a(n-3); a(1)=0, a(2)=1, a(3)=1
 * Range:     u8 n <= 9, u16 n <= 14, u32 n <= 21, u64 n <= 35, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* array of computed values, indexed by n-1 */
    static size_t cap = 0;        /* current allocated size of cache      */
    static idx_t maxComp = 0;     /* highest n that has been computed     */

    if (n == 0)          /* in case lower indices are requested */
        return 0;

    /* Ensure capacity for index n-1 (since a(n) is stored at cache[n-1]) */
    if ((size_t)n > cap) {
        size_t newCap = cap ? cap * 2 : 4;
        while (newCap < (size_t)n)
            newCap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, newCap * sizeof(seq_t));
        if (!tmp)                 /* out of memory – return 0 as a safe fallback */
            return 0;
        cache = tmp;
        cap = newCap;
    }

    /* Initialise base values on first use */
    if (maxComp == 0) {
        if (cap < 3) {          /* make room for a(1), a(2), a(3) */
            seq_t *tmp = (seq_t *)realloc(cache, (size_t)3 * sizeof(seq_t));
            if (!tmp)
                return 0;
            cache = tmp;
            cap = 3;
        }
        cache[0] = 0;           /* a(1) */
        cache[1] = 1;           /* a(2) */
        cache[2] = 1;           /* a(3) */
        maxComp = 3;
    }

    /* Compute any missing values up to n */
    for (idx_t i = maxComp + 1; i <= n; ++i) {
        /* cache stores a(k) at cache[k-1] */
        seq_t a_i_2 = cache[(size_t)(i - 3)]; /* a(i-2) */
        seq_t a_i_3 = cache[(size_t)(i - 4)]; /* a(i-3) */
        cache[(size_t)(i - 1)] = ((seq_t)(i - 2)) * a_i_2 + a_i_3;
    }

    if (n > maxComp)
        maxComp = n;

    return cache[(size_t)(n - 1)];
}
