/*
 * A170094  Number of reduced words of length n in Coxeter group on 37
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^37 = I.
 * https://oeis.org/A170094
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=37, a(2)=1332, a(3)=47952, a(4)=1726272,
 *            a(5)=62145792, a(6)=2237248512, a(7)=80540946432,
 *            a(8)=2899474071552, a(9)=104381066575872,
 *            a(10)=3757718396731392, a(11)=135277862282330112,
 *            a(12)=4870003042163884032, a(13)=175320109517899825152,
 *            a(14)=6311523942644393705472. For n ≥ 37, the...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static size_t cap   = 0;  /* allocated capacity */
    static size_t len   = 0;  /* number of terms already computed */

    /* ensure we have storage for indices up to n */
    if (n >= cap) {
        size_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) { /* allocation failed – aborting as per typical practice */
            abort();
        }
        cache = tmp;
        cap   = new_cap;
    }

    /* compute missing terms up to n */
    if (n >= len) {
        for (idx_t i = len; i <= n; ++i) {
            seq_t sum = 0;

            /* add 35 * a(i-k) for k = 1..36 */
            for (idx_t k = 1; k <= 36; ++k) {
                if (i >= k) {
                    sum += cache[i - k] * (seq_t)35;
                }
            }

            /* subtract 630 * a(i-37) if i ≥ 37 */
            if (i >= 37) {
                sum -= cache[i - 37] * (seq_t)630;
            }

            /* right‑hand side from the numerator (p_i) */
            seq_t rhs;
            if (i == 0)          rhs = 1;
            else if (i <= 36)    rhs = 2;
            else if (i == 37)    rhs = 1;
            else                 rhs = 0;

            cache[i] = sum + rhs;
        }
        len = n + 1;
    }

    return cache[n];
}
