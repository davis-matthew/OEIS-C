/*
 * A205251  Number of (n+1) X 5 0..1 arrays with the number of clockwise edge
 *          increases in every 2 X 2 subblock the same.
 * https://oeis.org/A205251
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Empirical recurrence for OEIS A205251 a(n) = 17*a(n-1) -
 *            81*a(n-2) + 157*a(n-3) -140*a(n-4) + 56*a(n-5) - 8*a(n-6)
 *            Initial values: a(1)=328, a(2)=3416, a(3)=36304, a(4)=387328,
 *            a(5)=4136296, a(6)=44183032
 * Range:     u8 none, u16 n <= 3, u32 n <= 7, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* initial terms stored 1‑based */
    static const seq_t init[7] = {0, 328, 3416, 36304, 387328, 4136296, 44183032};

    if (n <= 6)
        return init[n];

    /* buffer of last six terms: buf[0]=a(k-6), buf[5]=a(k-1) */
    seq_t buf[6];
    buf[0] = init[1];
    buf[1] = init[2];
    buf[2] = init[3];
    buf[3] = init[4];
    buf[4] = init[5];
    buf[5] = init[6];

    for (idx_t k = 7; k <= n; ++k) {
        seq_t p = 17 * buf[5]        /* 17*a(k-1) */
              + 157 * buf[3]         /* +157*a(k-3) */
              + 56  * buf[1];        /* +56*a(k-5) */

        seq_t nsum = 81 * buf[4]     /* 81*a(k-2) */
                 + 140 * buf[2]      /* +140*a(k-4) */
                 + 8  * buf[0];      /* +8*a(k-6) */

        seq_t new_val = p - nsum;   /* recurrence */

        /* shift buffer left and add new_val */
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = buf[4];
        buf[4] = buf[5];
        buf[5] = new_val;
    }

    return buf[5];
}
