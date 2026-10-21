/*
 * A086247  Differences of successive 7-smooth numbers.
 * https://oeis.org/A086247
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = A002473(n+1) – A002473(n) (difference of successive
 *            7‑smooth numbers)
 * Range:     u8 n <= 74, u16 n <= 1180, u32 n <= 8519, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static storage for 7‑smooth numbers (0‑based) */
    static seq_t *smooth = NULL;          /* smooth[k] = the k‑th 7‑smooth number */
    static idx_t  smooth_len = 0;         /* current number of stored elements */
    static idx_t  i2 = 0, i3 = 0, i5 = 0, i7 = 0;   /* pointers for the 4 generators */

    /* initialise on first call */
    if (!smooth) {
        smooth_len = 1;                  /* smooth[0] = 1 */
        smooth = (seq_t *)malloc(sizeof(seq_t));
        if (!smooth) abort();
        smooth[0] = (seq_t)1;
        i2 = i3 = i5 = i7 = 0;
    }

    /* generate 7‑smooth numbers until we have the n‑th and (n‑1)-th terms */
    while (smooth_len <= n) {
        /* candidates from each generator */
        seq_t next2 = smooth[i2] * (seq_t)2;
        seq_t next3 = smooth[i3] * (seq_t)3;
        seq_t next5 = smooth[i5] * (seq_t)5;
        seq_t next7 = smooth[i7] * (seq_t)7;

        /* smallest candidate */
        seq_t next = next2;
        if (next3 < next) next = next3;
        if (next5 < next) next = next5;
        if (next7 < next) next = next7;

        /* append the new smooth number */
        seq_t *tmp = (seq_t *)realloc(smooth, (smooth_len + 1) * sizeof(seq_t));
        if (!tmp) abort();
        smooth = tmp;
        smooth[smooth_len] = next;
        smooth_len++;

        /* advance pointers that contributed the chosen value */
        if (next == next2) i2++;
        if (next == next3) i3++;
        if (next == next5) i5++;
        if (next == next7) i7++;
    }

    /* a(n) = smooth[n] - smooth[n-1]   (n ≥ 1) */
    return smooth[n] - smooth[n - 1];
}
