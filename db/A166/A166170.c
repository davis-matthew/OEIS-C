/*
 * A166170  Number of reduced words of length n in Coxeter group on 38
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^10 = I.
 * https://oeis.org/A166170
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 36·(a(n-1)+…+a(n-9)) – 666·a(n-10)  for n ≥ 11
   Initial values:
     a(0)=1, a(1)=38, a(2)=1406, a(3)=52022, a(4)=1924814,
     a(5)=71218118, a(6)=2635070366, a(7)=97497603542,
     a(8)=3607411331054, a(9)=133474219248998,
     a(10)=4938546112212223 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *buf = NULL;
    static idx_t buffer_cap = 0;
    static idx_t next_compute = 0;   /* first index not yet known */

    /* initialize the table with the first 11 values (0 … 10) */
    if (buf == NULL) {
        buffer_cap = 64;
        if (buffer_cap < 32) buffer_cap = 32;
        buf = (seq_t *)malloc(buffer_cap * sizeof(seq_t));
        if (!buf) exit(1);
        buf[0] = (seq_t)1ULL;
        buf[1] = (seq_t)38ULL;
        buf[2] = (seq_t)1406ULL;
        buf[3] = (seq_t)52022ULL;
        buf[4] = (seq_t)1924814ULL;
        buf[5] = (seq_t)71218118ULL;
        buf[6] = (seq_t)2635070366ULL;
        buf[7] = (seq_t)97497603542ULL;
        buf[8] = (seq_t)3607411331054ULL;
        buf[9] = (seq_t)133474219248998ULL;
        buf[10] = (seq_t)4938546112212223ULL;
        /* we already know indices 0 … 10 */
        next_compute = 11;
    }

    /* if the requested index is already known, return it */
    if (n < next_compute) return buf[n];

    /* make sure the buffer is large enough */
    if (buffer_cap <= n) {
        idx_t new_cap = buffer_cap;
        while (new_cap <= n) {
            new_cap *= 2;
            if (new_cap == 0) { /* avoid infinite loop on overflow */
                new_cap = n + 1;
                break;
            }
        }
        seq_t *new_buf = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_buf) exit(1);
        for (idx_t i = 0; i < next_compute; ++i) new_buf[i] = buf[i];
        free(buf);
        buf = new_buf;
        buffer_cap = new_cap;
    }

    /* compute missing values using the recurrence */
    const seq_t mult36 = (seq_t)36ULL;
    const seq_t mult666 = (seq_t)666ULL;
    for (idx_t i = next_compute; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        sum += mult36 * buf[i - 1];
        sum += mult36 * buf[i - 2];
        sum += mult36 * buf[i - 3];
        sum += mult36 * buf[i - 4];
        sum += mult36 * buf[i - 5];
        sum += mult36 * buf[i - 6];
        sum += mult36 * buf[i - 7];
        sum += mult36 * buf[i - 8];
        sum += mult36 * buf[i - 9];
        sum -= mult666 * buf[i - 10];
        buf[i] = sum;
    }
    next_compute = n + 1;   /* indices 0 … n are now known */

    return buf[n];
}
