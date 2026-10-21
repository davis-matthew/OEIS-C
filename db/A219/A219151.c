/*
 * A219151  Unchanging value maps: number of n X 3 binary arrays indicating
 *          the locations of corresponding elements unequal to no horizontal,
 *          antidiagonal or vertical neighbor in a random 0..1 n X 3 array.
 * https://oeis.org/A219151
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 6*a(n-2) + 14*a(n-3) - 2*a(n-4) + a(n-5) +
 *            11*a(n-6) - 9*a(n-7) + 6*a(n-8) - 3*a(n-9) - 4*a(n-10) Empirical
 *            recurrence; initial values: a(1)=4, a(2)=9, a(3)=32, a(4)=104,
 *            a(5)=360, a(6)=1234, a(7)=4224, a(8)=14421, a(9)=49292,
 *            a(10)=168568
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 36, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* memoization storage */
    static seq_t *cache = NULL;       /* 1‑based array of terms */
    static idx_t  max_computed = 0;   /* highest n for which cache[n] is valid */

    if (n < 1) return 0;              /* undefined for n < 1, return 0 as dummy */

    /* Expand cache if necessary */
    if (n > max_computed) {
        idx_t new_len = n;
        if (!cache)
            cache = (seq_t *)malloc((new_len + 1) * sizeof(seq_t));
        else
            cache = (seq_t *)realloc(cache, (new_len + 1) * sizeof(seq_t));

        max_computed = new_len;

        /* initialise first ten terms if they are not yet known */
        if (max_computed >= 1) {
            static const seq_t init10[10] = {
                (seq_t)4, (seq_t)9, (seq_t)32, (seq_t)104, (seq_t)360,
                (seq_t)1234, (seq_t)4224, (seq_t)14421, (seq_t)49292,
                (seq_t)168568
            };
            idx_t up = (max_computed < 10) ? max_computed : 10;
            for (idx_t i = 1; i <= up; ++i)
                cache[i] = init10[i - 1];
        }

        /* Compute new terms using the empirical recurrence */
        for (idx_t i = 11; i <= max_computed; ++i) {
            seq_t pos = cache[i-1] * (seq_t)4
                      + cache[i-3] * (seq_t)14
                      +          cache[i-5]
                      + cache[i-6] * (seq_t)11
                      + cache[i-8] * (seq_t)6;
            seq_t neg = cache[i-2] * (seq_t)6
                      + cache[i-4] * (seq_t)2
                      + cache[i-7] * (seq_t)9
                      + cache[i-9] * (seq_t)3
                      + cache[i-10] * (seq_t)4;
            cache[i] = pos - neg;
        }
    }

    return cache[n];
}
