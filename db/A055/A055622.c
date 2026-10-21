/*
 * A055622  Minimum solution for tri-color tower of Hanoi, restricted so like
 *          colors can't be together.
 * https://oeis.org/A055622
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 2*a(n-2) + 7*a(n-3) - 5*a(n-4) - 8*a(n-5) -
 *            14*a(n-6) + 2*a(n-7) + 4*a(n-8) + 4*a(n-9) + 8*a(n-10) Initial
 *            values: a(1)=1, a(2)=3, a(3)=7, a(4)=19, a(5)=43, a(6)=99,
 *            a(7)=235, a(8)=535, a(9)=1239, a(10)=2879
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 26, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* First ten terms */
    static const seq_t init[10] = {
        (seq_t)1,   (seq_t)3,   (seq_t)7,
        (seq_t)19,  (seq_t)43,  (seq_t)99,
        (seq_t)235, (seq_t)535, (seq_t)1239,
        (seq_t)2879
    };

    if (n <= 10)
        return init[n - 1];

    /* Holds a(n-10) .. a(n-1) */
    seq_t last[10];
    for (size_t i = 0; i < 10; ++i)
        last[i] = init[i];

    seq_t next;
    for (idx_t k = 11; k <= n; ++k) {
        /* Compute a(k) from a(k-1) .. a(k-10) */
        seq_t sum_pos = last[9] // a(k-1)
                      + (seq_t)2 * last[8]      // a(k-2)
                      + (seq_t)7 * last[7]      // a(k-3)
                      + (seq_t)2 * last[3]      // a(k-7)
                      + (seq_t)4 * last[2]      // a(k-8)
                      + (seq_t)4 * last[1]      // a(k-9)
                      + (seq_t)8 * last[0];     // a(k-10)
        seq_t sum_neg = (seq_t)5 * last[6]      // a(k-4)
                      + (seq_t)8 * last[5]      // a(k-5)
                      + (seq_t)14 * last[4];    // a(k-6)

        next = sum_pos - sum_neg; // all values fit in seq_t

        /* Shift the window: drop a(k-10), keep a(k-9)..a(k) */
        for (size_t i = 0; i < 9; ++i)
            last[i] = last[i + 1];
        last[9] = next;
    }

    return last[9];
}
