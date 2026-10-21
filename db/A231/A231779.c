/*
 * A231779  Number of n X 2 0..2 arrays with no element having a strict
 *          majority of its horizontal and vertical neighbors equal to itself
 *          plus one mod 3, with upper left element zero (rock paper and
 *          scissors drawn positions).
 * https://oeis.org/A231779
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 6*a(n-1) - 8*a(n-2) + 2*a(n-3) + 3*a(n-4) - a(n-5)
 * initial values: a(1)=1,a(2)=17,a(3)=74,a(4)=315,a(5)=1343,a(6)=5734   */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache: cache[i] holds a(i+1) for i>=0 */
    static seq_t *cache = NULL;
    static idx_t max_n = 0;   /* highest n cached so far */

    if (n == 0)
        return 0;   /* undefined, but return 0 for safety */

    /* grow cache if necessary */
    if (n > max_n) {
        size_t new_cap = n;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) { /* allocation failure */
            /* fallback: return 0; */
            return 0;
        }
        cache = new_cache;

        /* fill new entries */
        for (idx_t k = max_n + 1; k <= n; ++k) {
            if (k == 1) {
                cache[0] = (seq_t)1;
            } else if (k == 2) {
                cache[1] = (seq_t)17;
            } else if (k == 3) {
                cache[2] = (seq_t)74;
            } else if (k == 4) {
                cache[3] = (seq_t)315;
            } else if (k == 5) {
                cache[4] = (seq_t)1343;
            } else if (k == 6) {
                cache[5] = (seq_t)5734;
            } else {
                /* recurrence:
                   a(k) = 6*a(k-1) - 8*a(k-2) + 2*a(k-3) + 3*a(k-4) - a(k-5)   */
                idx_t i = k - 1;              /* index in cache for a(k) */
                seq_t a1 = cache[i - 1];      /* a(k-1) */
                seq_t a2 = cache[i - 2];      /* a(k-2) */
                seq_t a3 = cache[i - 3];      /* a(k-3) */
                seq_t a4 = cache[i - 4];      /* a(k-4) */
                seq_t a5 = cache[i - 5];      /* a(k-5) */

                /* compute using unsigned arithmetic; intermediate sums remain non‑negative
                   due to the recurrence structure. */
                seq_t val = a1 * (seq_t)6;
                val += a3 * (seq_t)2;
                val += a4 * (seq_t)3;
                val -= a2 * (seq_t)8;
                val -= a5;
                cache[i] = val;
            }
        }
        max_n = n;
    }

    return cache[n - 1];
}
