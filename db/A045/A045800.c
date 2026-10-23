/*
 * A045800  0-ish numbers (end in 01, 07, 43, 49).
 * https://oeis.org/A045800
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 1 + 100*floor((n-1)/4) + offset, where offset = 0 for
 *            (n-1)%4 = 0, 6 for (n-1)%4 = 1, 42 for (n-1)%4 = 2, 48 for
 *            (n-1)%4 = 3
 * Range:     u8 n <= 12, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;                 /* undefined for n < 1, return 0 safely */

    idx_t m = n - 1;                       /* number of differences to accumulate   */
    seq_t cycles = ((seq_t)m / 4) * (seq_t)100;  /* each full 4‑step cycle adds 100 */
    idx_t rem = m % 4;

    seq_t sum_delta = cycles;
    switch (rem) {
        case 1: sum_delta += 6;  break;
        case 2: sum_delta += 42; break;
        case 3: sum_delta += 48; break;
        default: /* rem == 0 */ break;
    }

    return (seq_t)1 + sum_delta;
}
