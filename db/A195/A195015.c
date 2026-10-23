/*
 * A195015  Main axis of the square spiral whose edges have length A195013 and
 *          whose vertices are the numbers A195014.
 * https://oeis.org/A195015
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n*(5*n+2) + 3*(-1)^n - 3) / 4
 * Range:     u8 n <= 4, u16 n <= 160, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          = (2*n*(5*n+2) - (n % 2 ? 6 : 0)) / 4   */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;               /* convert index to seq_t */
    seq_t term = 2 * n_seq * (5 * n_seq + 2);  /* 2*n*(5*n+2) */
    if (n & 1) {                          /* odd n => subtract 6 */
        term -= (seq_t)6;
    }
    /* division by 4 – guaranteed to be integer */
    return term / (seq_t)4;
}
