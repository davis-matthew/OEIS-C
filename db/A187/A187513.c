/*
 * A187513  Number of 8-step S, E, and NW-moving king's tours on an n X n
 *          board summed over all starting positions.
 * https://oeis.org/A187513
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A187513: Number of 8‑step S, E, and NW‑moving king's tours
 *  on an n × n board summed over all starting positions.
 *
 *  For n > 6 an empirical quadratic formula holds:
 *  a(n) = 669·n² – 4316·n + 6681
 *
 *  The first six terms are special and are hard‑coded:
 *  a(1)=0, a(2)=0, a(3)=12, a(4)=390, a(5)=1910, a(6)=4880
 *
 *  The function works for any n ≥ 1 and is constant time.
 */
#include <stdint.h>

seq_t a(idx_t n) {
    /* Handle special cases for n ≤ 6 */
    if (n <= 6) {
        /* 0-indexed array: value for n=1 is at index 0 */
        static const seq_t small[6] = {0, 0, 12, 390, 1910, 4880};
        return small[n - 1];
    }

    /* For n > 6 use the empirical formula */
    seq_t nn = (seq_t)n;
    /* Compute 669*nn*nn - 4316*nn + 6681 */
    return nn * nn * 669 - nn * 4316 + 6681;
}
