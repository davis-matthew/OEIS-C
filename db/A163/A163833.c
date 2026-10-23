/*
 * A163833  a(n) = n*(6*n^2 + 15*n + 5)/2.
 * https://oeis.org/A163833
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(6*n^2 + 15*n + 5)/2
 * Range:     u8 n <= 3, u16 n <= 27, u32 n <= 909, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t ns = (seq_t)n;
    seq_t t = (seq_t)6 * ns * ns;  /* 6*n^2 */
    t += (seq_t)15 * ns;           /* +15*n */
    t += (seq_t)5;                 /* +5 */
    return ns * t / (seq_t)2;      /* n*(t)/2 */
}
