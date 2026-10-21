/*
 * A055852  Convolution of A055589 with A011782.
 * https://oeis.org/A055852
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 0 a(n) = sum_{k=0}^{min(5,n-1)} (-1)^k * C(5,k) *
 *            C(5+n-1-k,5) * 2^(n-1-k) Equivalently, for n ≥ 7 a(n) =
 *            12*a(n-1) - 60*a(n-2) + 160*a(n-3) - 240*a(n-4) + 192*a(n-5) -
 *            64*a(n-6) with a(6)=1683 obtained from the non‑homogeneous part
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 21, u64 n <= 48, u128 n <= 106,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t base[7] = {0, 1, 7, 34, 138, 501, 1683};
    if (n < 7)
        return base[n];

    /* Sliding window: last[0] = a(i-6), ..., last[5] = a(i-1) */
    seq_t last[6];
    last[0] = base[1];   /* a(1) */
    last[1] = base[2];   /* a(2) */
    last[2] = base[3];   /* a(3) */
    last[3] = base[4];   /* a(4) */
    last[4] = base[5];   /* a(5) */
    last[5] = base[6];   /* a(6) */

    for (idx_t i = 7; i <= n; ++i) {
        /* Positive and negative parts are kept separate to avoid
           unsigned underflow.  The recurrence guarantees pos >= neg. */
        seq_t pos = (seq_t)12 * last[5]
                  + (seq_t)160 * last[3]
                  + (seq_t)192 * last[1];
        seq_t neg = (seq_t)60 * last[4]
                  + (seq_t)240 * last[2]
                  + (seq_t)64 * last[0];
        seq_t term = pos - neg;

        /* Shift the window: drop a(i-6), shift others left, insert term */
        for (size_t k = 0; k < 5; ++k)
            last[k] = last[k + 1];
        last[5] = term;
    }
    return last[5];
}
