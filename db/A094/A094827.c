/*
 * A094827  Number of (s(0), s(1), ..., s(2n+1)) such that 0 < s(i) < 9 and
 *          |s(i) - s(i-1)| = 1 for i = 1,2,...,2n+1, s(0) = 1, s(2n+1) = 4.
 * https://oeis.org/A094827
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) - 15*a(n-2) + 10*a(n-3) - a(n-4) (n>=5) with
 *            a(1)=1, a(2)=4, a(3)=14, a(4)=48
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 36, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;     /* offset is 1, n>=1 in practice */

    static seq_t *cache = NULL;
    static idx_t len   = 0;          /* number of terms computed, i.e. cache[k-1] = a(k) */
    static idx_t cap   = 0;          /* allocated size of cache */

    /* ensure capacity */
    if (n > cap) {
        idx_t new_cap = (cap ? cap * 2 : 1);
        while (new_cap < n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp) return (seq_t)0;  /* allocation failed */
        cache = tmp;
        cap = new_cap;
    }

    /* compute missing terms */
    if (n > len) {
        for (idx_t k = len + 1; k <= n; ++k) {
            if (k == 1) cache[0] = (seq_t)1;
            else if (k == 2) cache[1] = (seq_t)4;
            else if (k == 3) cache[2] = (seq_t)14;
            else if (k == 4) cache[3] = (seq_t)48;
            else {
                /* recurrence: a(k) = 7*a(k-1) - 15*a(k-2) + 10*a(k-3) - a(k-4) */
                cache[k - 1] =
                    (seq_t)(7 * cache[k - 2]
                          - 15 * cache[k - 3]
                          + 10 * cache[k - 4]
                          - cache[k - 5]);
            }
        }
        len = n;
    }

    return cache[n - 1];
}
