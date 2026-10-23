/*
 * A104651  Numbers n such that n^2 = 29*k^2 + 29*k +1, k sequence = A104652.
 * https://oeis.org/A104651
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1, a(2)=175, a(3)=16589,
   a(4)=19602*a(1)+a(3), a(5)=19602*a(2)+a(2),
   a(6)=19602*a(3)+a(1), and for n≥7,
   a(n)=19602*a(n-3)-a(n-6). */
#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;               /* offset is 1 */

    static seq_t *data = NULL;
    static idx_t cap   = 0;

    /* Ensure we have space for indices up to n.  Use exponential growth
       to keep realloc costs low. */
    if (n >= cap) {
        idx_t newcap = (cap == 0) ? 16 : cap * 2;
        while (newcap <= n) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(data, sizeof(seq_t) * newcap);
        data = tmp;
        for (idx_t i = cap; i < newcap; ++i) data[i] = 0;
        cap = newcap;
    }

    /* Initialise the first six terms on demand.  Remaining terms are
       computed via the recurrence and cached. */
    if (data[1] == 0) data[1] = (seq_t)1;
    if (data[2] == 0) data[2] = (seq_t)175;
    if (data[3] == 0) data[3] = (seq_t)16589;
    if (data[4] == 0) data[4] = (seq_t)(19602 * data[1] + data[3]);
    if (data[5] == 0) data[5] = (seq_t)(19602 * data[2] + data[2]);
    if (data[6] == 0) data[6] = (seq_t)(19602 * data[3] + data[1]);

    for (idx_t i = 7; i <= n; ++i) {
        if (data[i] == 0) {
            seq_t prod = data[i - 3] * (seq_t)19602;
            data[i] = prod - data[i - 6];
        }
    }
    return data[n];
}
