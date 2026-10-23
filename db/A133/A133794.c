/*
 * A133794  Times on a 12-hour digital clock with all digits in {1, 2, 3, 4,
 *          5, 6}.
 * https://oeis.org/A133794
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, fini, full, nonn, base, less, dumb
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 7440, u64 n <= 7440, u128 n <= 7440,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A133794: integers obtained by concatenating the digits of all
   possible times on a 12‑hour digital clock that use only the digits
   1‑6. The terms are sorted in increasing order. */
#include <stdlib.h>

static int seq_compare(const void *a, const void *b)
{
    const seq_t *x = (const seq_t *)a;
    const seq_t *y = (const seq_t *)b;
    return (*x < *y) ? -1 : (*x > *y) ? 1 : 0;
}

seq_t a(idx_t n)
{
    static seq_t *arr = NULL;
    static idx_t count = 0;
    if (!arr) {
        arr = (seq_t *)malloc(sizeof(seq_t) * 7440);
        if (!arr) return 0;
        const int hours[] = {1,2,3,4,5,6,11,12};
        const size_t hour_cnt = sizeof(hours) / sizeof(hours[0]);

        /* minute and second possibilities (tens 1–5, units 1–6) */
        const int minutes[30] = {
            11,12,13,14,15,16,
            21,22,23,24,25,26,
            31,32,33,34,35,36,
            41,42,43,44,45,46,
            51,52,53,54,55,56
        };
        const size_t minute_cnt = 30;

        size_t idx = 0;
        for (size_t hi = 0; hi < hour_cnt; ++hi) {
            int h = hours[hi];
            if (h < 10) {
                /* single‑digit hour: 3‑ and 5‑digit times */
                for (size_t mi = 0; mi < minute_cnt; ++mi) {
                    int m = minutes[mi];
                    seq_t val3 = (seq_t)h * 100 + m;
                    arr[idx++] = val3;
                    for (size_t si = 0; si < minute_cnt; ++si) {
                        int s = minutes[si];
                        seq_t val5 = (seq_t)h * 10000 + (seq_t)m * 100 + s;
                        arr[idx++] = val5;
                    }
                }
            } else {
                /* double‑digit hour: 4‑ and 6‑digit times */
                for (size_t mi = 0; mi < minute_cnt; ++mi) {
                    int m = minutes[mi];
                    seq_t val4 = (seq_t)h * 100 + m;
                    arr[idx++] = val4;
                    for (size_t si = 0; si < minute_cnt; ++si) {
                        int s = minutes[si];
                        seq_t val6 = (seq_t)h * 10000 + (seq_t)m * 100 + s;
                        arr[idx++] = val6;
                    }
                }
            }
        }
        count = idx;            /* should be 7440 */
        qsort(arr, count, sizeof(seq_t), seq_compare);
    }
    if (n == 0 || n > count) return 0;
    return arr[n - 1];
}
