/*
 * A145812  Odd positive integers a(n) such that for every odd integer m > 1
 *          there exists a unique representation of m as a sum of the form
 *          a(l) + 2a(s).
 * https://oeis.org/A145812
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = 1 + 2 * g, where g is obtained by interpreting the binary
 *            representation of (n-1) as a base‑4 number. That is, if (n-1) =
 *            Σ b_j·2^j with bits b_j∈{0,1}, then g = Σ b_j·4^j and a(n) = 1 +
 *            2·g. This yields exactly the OEIS sequence A145812
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* m holds n-1 as an index type (uint64_t). */
    idx_t m = n - 1;
    seq_t g = 0;        /* base‑4 value of the bits of n-1. */
    seq_t power = 1;    /* 4^j, starting with j = 0.            */

    while (m) {
        if (m & 1)          /* bit j is set? */
            g += power;     /* add 4^j to g.                    */
        m >>= 1;           /* move to next bit.                 */
        power <<= 2;       /* next power of 4.                  */
    }

    /* a(n) = 1 + 2·g  ->  (1) + (g << 1)  */
    return ((seq_t)1) + (g << 1);
}
