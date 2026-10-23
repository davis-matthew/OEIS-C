/*
 * A162985  Number of Dyck paths with no UUU's and no DDD's of semilength n
 *          and having no UUDUDD's (U=(1,1), D=(1,-1)).
 * https://oeis.org/A162985
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of Dyck paths of semilength n with no UUU, no DDD,
 *            and no UUDUDD. Recurrence: for n ≥ 6 (n+3)·a(n) = (2n+3)·a(n‑1)
 *            + n·a(n‑2) + (n‑3)·a(n‑4) + (2n‑9)·a(n‑5) – (n‑6)·a(n‑6) Initial
 *            values: a(0)=1, a(1)=1, a(2)=2, a(3)=3, a(4)=6, a(5)=12
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;     /* array of a[0..size-1] */
    static idx_t size = 0;          /* number of stored terms */

    /* allocate or extend cache if necessary */
    if (n + 1 > size) {
        idx_t newsize = n + 1;
        if (cache == NULL) {
            cache = (seq_t *)malloc(newsize * sizeof(seq_t));
        } else {
            cache = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
        }
        if (cache == NULL) {
            /* allocation failed – abort */
            return (seq_t)0;
        }

        /* fill missing terms */
        for (idx_t i = size; i <= n; ++i) {
            /* initial values */
            if (i == 0) {
                cache[0] = (seq_t)1;
            } else if (i == 1) {
                cache[1] = (seq_t)1;
            } else if (i == 2) {
                cache[2] = (seq_t)2;
            } else if (i == 3) {
                cache[3] = (seq_t)3;
            } else if (i == 4) {
                cache[4] = (seq_t)6;
            } else if (i == 5) {
                cache[5] = (seq_t)12;
            } else {
                /* recurrence for i ≥ 6 */
                seq_t sum = (seq_t)(2 * i + 3) * cache[i - 1]
                          + (seq_t)i * cache[i - 2]
                          + (seq_t)(i - 3) * cache[i - 4]
                          + (seq_t)(2 * i - 9) * cache[i - 5];
                sum -= (seq_t)(i - 6) * cache[i - 6];
                cache[i] = sum / (seq_t)(i + 3);
            }
        }

        size = newsize;
    }

    return cache[n];
}
