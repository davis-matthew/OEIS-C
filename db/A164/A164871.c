/*
 * A164871  The number of permutations of length n that can be sorted by 3 pop
 *          stacks in parallel.
 * https://oeis.org/A164871
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 14*a(n-1) - 75*a(n-2) + 201*a(n-3) - 288*a(n-4) +
 *            216*a(n-5) - 72*a(n-6) for n > 6 Initial values: a(0)=1, a(1)=1,
 *            a(2)=2, a(3)=6, a(4)=24, a(5)=118, a(6)=644
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 15, u64 n <= 28, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Base cases for n = 0..6 */
    static const seq_t base[7] = {
        (seq_t)1,   /* a(0) */
        (seq_t)1,   /* a(1) */
        (seq_t)2,   /* a(2) */
        (seq_t)6,   /* a(3) */
        (seq_t)24,  /* a(4) */
        (seq_t)118, /* a(5) */
        (seq_t)644  /* a(6) */
    };

    if (n <= 6)
        return base[n];

    /* Maintain the last six values:
       vals[0] = a(i-6), vals[1] = a(i-5), ..., vals[5] = a(i-1)   for current i */
    seq_t vals[6];
    vals[0] = base[1]; /* a(1) */
    vals[1] = base[2]; /* a(2) */
    vals[2] = base[3]; /* a(3) */
    vals[3] = base[4]; /* a(4) */
    vals[4] = base[5]; /* a(5) */
    vals[5] = base[6]; /* a(6) */

    for (idx_t i = 7; i <= n; ++i) {
        seq_t next =
              (seq_t)14 * vals[5]  /* a(i-1) */
            - (seq_t)75 * vals[4]  /* a(i-2) */
            + (seq_t)201 * vals[3] /* a(i-3) */
            - (seq_t)288 * vals[2] /* a(i-4) */
            + (seq_t)216 * vals[1] /* a(i-5) */
            - (seq_t)72  * vals[0];/* a(i-6) */

        /* Shift window: drop oldest, shift others left, insert new term */
        vals[0] = vals[1];
        vals[1] = vals[2];
        vals[2] = vals[3];
        vals[3] = vals[4];
        vals[4] = vals[5];
        vals[5] = next;
    }

    return vals[5];  /* a(n) */
}
