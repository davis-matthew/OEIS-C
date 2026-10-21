/*
 * A200554  Number of -n..n arrays x(0..3) of 4 elements with zero sum and
 *          nonzero second differences.
 * https://oeis.org/A200554
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + 2*a(n-3) - 4*a(n-4) + 2*a(n-5) -
 *            a(n-6) + 2*a(n-7) - a(n-8) Initial values: a(1)=14, a(2)=76,
 *            a(3)=200, a(4)=446, a(5)=836, a(6)=1368, a(7)=2134, a(8)=3140
 * Range:     u8 n <= 3, u16 n <= 22, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* First 8 terms are hard‑coded (they are small constants). */
    static const seq_t base[8] = {
        (seq_t)14,
        (seq_t)76,
        (seq_t)200,
        (seq_t)446,
        (seq_t)836,
        (seq_t)1368,
        (seq_t)2134,
        (seq_t)3140
    };

    if (n <= 8) {
        /* The type seq_t may be an arbitrary‑precision integer. */
        return base[n - 1];
    }

    seq_t last[8];
    for (idx_t i = 0; i < 8; ++i) {
        last[i] = base[i];
    }

    seq_t cur = 0;
    for (idx_t i = 9; i <= n; ++i) {
        /* Compute a(i) from the 8 previous terms. */
        cur = last[7] * ((seq_t)2)             /* 2*a(i-1) */
              - last[6]                         /* -a(i-2)  */
              + last[5] * ((seq_t)2)            /* +2*a(i-3) */
              - last[4] * ((seq_t)4)            /* -4*a(i-4) */
              + last[3] * ((seq_t)2)            /* +2*a(i-5) */
              - last[2]                         /* -a(i-6)  */
              + last[1] * ((seq_t)2)            /* +2*a(i-7) */
              - last[0];                       /* -a(i-8)  */

        /* Shift the saved terms: discard the oldest (a(i-8)). */
        for (idx_t j = 0; j < 7; ++j) {
            last[j] = last[j + 1];
        }
        last[7] = cur;                          /* store the newest term */
    }

    return cur;
}
