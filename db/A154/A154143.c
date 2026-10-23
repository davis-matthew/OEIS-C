/*
 * A154143  Indices k such that 10 plus the k-th triangular number is a
 *          perfect square.
 * https://oeis.org/A154143
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-2) - a(n-4) + 2 for n > 4, * with initial values
 *            a(1)=3, a(2)=5, a(3)=26, a(4)=36
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cache[0] holds a(1), cache[1] holds a(2), … */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;          /* number of terms currently stored */

    if (n == 0)
        return 0;                        /* sequence undefined for n = 0 */

    /* extend cache if necessary */
    if (n > cache_len) {
        idx_t new_len = n;
        seq_t *tmp = (seq_t *)realloc(cache,
                                       new_len * sizeof(seq_t));
        if (!tmp)          /* allocation failure */
            return 0;
        cache = tmp;

        /* fill new entries using the recurrence */
        for (idx_t i = cache_len; i < new_len; ++i) {   /* i is 0‑based index */
            seq_t val;
            idx_t k = i + 1;                            /* sequence index */
            if (k == 1) {
                val = (seq_t)3;
            } else if (k == 2) {
                val = (seq_t)5;
            } else if (k == 3) {
                val = (seq_t)26;
            } else if (k == 4) {
                val = (seq_t)36;
            } else {
                /* a(k) = 6*a(k-2) - a(k-4) + 2 */
                val = ((seq_t)6 * cache[i - 2]) - cache[i - 4] + (seq_t)2;
            }
            cache[i] = val;
        }
        cache_len = new_len;
    }

    return cache[n - 1];
}
