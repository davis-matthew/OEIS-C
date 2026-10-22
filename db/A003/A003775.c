/*
 * A003775  Number of perfect matchings (or domino tilings) in P_5 X P_2n.
 * https://oeis.org/A003775
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 15*a(n-1) - 32*a(n-2) + 15*a(n-3) - a(n-4), a(0) = 1,
 *            a(1) = 8, a(2) = 95, a(3) = 1183
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t w = (seq_t)1;    /* a(i-3) */
    seq_t x = (seq_t)8;    /* a(i-2) */
    seq_t y = (seq_t)95;   /* a(i-1) */
    seq_t z = (seq_t)1183; /* a(i)   */
    idx_t i;
    if (n == 0) return w;
    if (n == 1) return x;
    if (n == 2) return y;
    for (i = 3; i < n; i++) {
        seq_t pos = (seq_t)15 * (z + x);
        seq_t neg = (seq_t)32 * y + w;
        seq_t next = pos - neg;
        w = x;
        x = y;
        y = z;
        z = next;
    }
    return z;
}
