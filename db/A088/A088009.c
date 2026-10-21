/*
 * A088009  Number of "sets of odd lists", cf. A000262.
 * https://oeis.org/A088009
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=a(1)=a(2)=1, a(3)=7 *
 *            a(n)=a(n-1)+2*(n-2)*(n-1)*a(n-2)+(n-2)*(n-1)*a(n-3) *
 *            -(n-4)*(n-3)*(n-2)*(n-1)*a(n-4) for n≥4 * This function memoises
 *            the values in a dynamically sized static array * to allow
 *            repeated calls with arbitrary n
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static memoisation array */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;     /* number of stored values (max index + 1) */
    static size_t cache_cap = 0;     /* allocated capacity */

    /* make cast to size_t safely */
    size_t idx = (size_t)n;

    /* extend cache if necessary */
    if (idx + 1 > cache_len) {
        /* increase capacity (double strategy) */
        size_t new_cap = cache_len ? cache_len << 1 : 1;
        while (new_cap < idx + 1) new_cap <<= 1;
        if (new_cap != cache_cap) {
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            cache = tmp;
            cache_cap = new_cap;
        }

        /* compute missing terms */
        for (size_t i = cache_len; i <= idx; ++i) {
            if (i == 0 || i == 1 || i == 2) {
                cache[i] = 1;
            } else if (i == 3) {
                cache[i] = 7;
            } else {
                /* all indices are >=4 here */
                seq_t term = cache[i - 1];

                seq_t a2 = cache[i - 2];
                seq_t a3 = cache[i - 3];
                seq_t a4 = cache[i - 4];

                seq_t m1 = (seq_t)(i - 2);
                seq_t m2 = (seq_t)(i - 1);
                seq_t m3 = (seq_t)(i - 3);
                seq_t m4 = (seq_t)(i - 4);

                /* 2*m1*m2*a2 */
                seq_t tmp = a2;
                tmp *= m1;
                tmp *= m2;
                tmp *= 2;
                term += tmp;

                /* m1*m2*a3 */
                tmp = a3;
                tmp *= m1;
                tmp *= m2;
                term += tmp;

                /* m1*m2*m3*m4*a4  (subtracted) */
                tmp = a4;
                tmp *= m1;
                tmp *= m2;
                tmp *= m3;
                tmp *= m4;
                term -= tmp;

                cache[i] = term;
            }
        }
        cache_len = idx + 1;
    }

    return cache[idx];
}
