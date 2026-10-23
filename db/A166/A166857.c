/*
 * A166857  Number of reduced words of length n in Coxeter group on 3
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^13 = I.
 * https://oeis.org/A166857
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   non-linear recurrence a(n) = 2*a(n-1) - a(n-2) + 2*a(n-3) -
 *            a(n-4) + 2*a(n-5) - a(n-6) + 2*a(n-7) - a(n-8) + 2*a(n-9) -
 *            a(n-10) + 2*a(n-11) - a(n-12) initial values: a(0)=1, a(1)=3,
 *            a(2)=6, a(3)=12, a(4)=24, a(5)=48, a(6)=96, a(7)=192, a(8)=384,
 *            a(9)=768, a(10)=1536, a(11)=3072, a(12)=6144 This r...
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const seq_t init_vals[13] = {
    (seq_t)1,   (seq_t)3,  (seq_t)6,   (seq_t)12,  (seq_t)24,
    (seq_t)48,  (seq_t)96, (seq_t)192, (seq_t)384, (seq_t)768,
    (seq_t)1536,(seq_t)3072,(seq_t)6144
};

seq_t a(idx_t n) {
    if (n < 13)
        return init_vals[n];

    /* ring buffer holding a[n-12] ... a[n-1] */
    seq_t last[12];
    /* initialise for m = 13: last[0] = a(1), last[11] = a(12) */
    for (idx_t i = 0; i < 12; ++i)
        last[i] = init_vals[i + 1];

    seq_t next_val = (seq_t)0;
    for (idx_t m = 13; m <= n; ++m) {
        next_val = last[11] + last[11];        /* 2*a(m-1)   */
        next_val -= last[10];                 /* -a(m-2)    */
        next_val += last[9] + last[9];        /* +2*a(m-3)  */
        next_val -= last[8];                  /* -a(m-4)    */
        next_val += last[7] + last[7];        /* +2*a(m-5)  */
        next_val -= last[6];                  /* -a(m-6)    */
        next_val += last[5] + last[5];        /* +2*a(m-7)  */
        next_val -= last[4];                  /* -a(m-8)    */
        next_val += last[3] + last[3];        /* +2*a(m-9)  */
        next_val -= last[2];                  /* -a(m-10)   */
        next_val += last[1] + last[1];        /* +2*a(m-11) */
        next_val -= last[0];                  /* -a(m-12)   */

        /* shift buffer left and insert new value */
        for (idx_t i = 0; i < 11; ++i)
            last[i] = last[i + 1];
        last[11] = next_val;
    }

    return next_val;
}
