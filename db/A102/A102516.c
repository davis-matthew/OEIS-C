/*
 * A102516  Sum C(n-3k,3k+1), k=0..floor(n/6).
 * https://oeis.org/A102516
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sum{k=0..floor(n/6)} C(n-3k, 3k+1)
 * Range:     u8 n <= 13, u16 n <= 24, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* seq_t and idx_t are supplied by the build; do not define them here. */
seq_t a(idx_t n)
{
    /* Base values for n = 0..5. */
    if (n < 6) {
        static const seq_t init[6] = { 0, 1, 2, 3, 4, 5 };
        return init[n];
    }

    /* Sliding window for the recurrence:
       a(n) = 3*a(n-1) - 3*a(n-2) + a(n-3) + a(n-6)   (n >= 6) */
    seq_t w[6];
    w[0] = 0;  /* a(0) */
    w[1] = 1;  /* a(1) */
    w[2] = 2;  /* a(2) */
    w[3] = 3;  /* a(3) */
    w[4] = 4;  /* a(4) */
    w[5] = 5;  /* a(5) */

    for (idx_t i = 6; i <= n; ++i) {
        seq_t next = 3 * w[5] - 3 * w[4] + w[3] + w[0];  /* a(i) */

        /* Shift window forward for the next iteration. */
        w[0] = w[1];
        w[1] = w[2];
        w[2] = w[3];
        w[3] = w[4];
        w[4] = w[5];
        w[5] = next;
    }

    return w[5];  /* a(n) */
}
