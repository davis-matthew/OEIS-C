/*
 * A040602  Continued fraction for sqrt(628).
 * https://oeis.org/A040602
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(n) are the terms of the continued‑fraction expansion of
 *            sqrt(628)
 * Range:     u8 none, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The algorithm uses the standard recurrence:
   a0 = ⌊√628⌋, m0 = 0, d0 = 1.
   For k≥0:
     mk+1 = dk * ak – mk
     dk+1 = (628 – mk+1²) / dk
     ak+1 = ⌊(a0 + mk+1)/dk+1⌋
   Then a(n) = ak.                                      */
#include <stdint.h>

seq_t a(idx_t n) {
    const seq_t D = (seq_t)628;

    /* Compute a0 = floor(sqrt(D)) by linear search (D is small). */
    seq_t a0 = 0;
    for (seq_t i = 1; i * i <= D; ++i) {
        a0 = i;
    }

    seq_t m = 0;
    seq_t d = 1;
    seq_t a_val = a0;          /* a0 corresponds to a(0) */

    if (n == 0) {
        return a_val;
    }

    for (idx_t k = 1; k <= n; ++k) {
        m = d * a_val - m;
        d = (D - m * m) / d;
        a_val = (a0 + m) / d;
    }

    return a_val;
}
