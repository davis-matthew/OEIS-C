/*
 * A086084  A086070 in binary.
 * https://oeis.org/A086084
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = A007088(A086070(n)), where A086070(n) generates binary
 *            strings of length L (L ≥ 2) with at most one zero (zero may be
 *            in the last position only when L = 2). These strings are listed
 *            in increasing decimal order, and A007088 converts a decimal
 *            number to its binary digits seen as a decimal in...
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n <= 1) return 1;            /* a(1) = 1 */

    idx_t rem = n - 1;               /* skip the first term */
    idx_t L   = 2;                   /* start with length 2 */

    /* Find the length L of the binary string for index n */
    while (1) {
        idx_t cnt = (L == 2) ? 2 : (L - 1);   /* terms of this length */
        if (rem <= cnt) break;
        rem -= cnt;
        ++L;
    }

    /* Determine the position of the zero (if any) */
    idx_t zeroIndex = rem - 1;                         /* 0‑based in this block */
    idx_t minZero   = (L == 2) ? 1 : (L - 2);          /* number of zero choices */
    idx_t zeroPos   = 0;                               /* 0 means no zero */
    if (zeroIndex < minZero)
        zeroPos = 2 + zeroIndex;                       /* zero at this position */

    /* Build the decimal integer that reads the binary bits as digits */
    seq_t value = 0;
    for (idx_t pos = 1; pos <= L; ++pos) {
        seq_t bit = 1;
        if (zeroPos && pos == zeroPos) bit = 0;
        value = value * 10 + bit;
    }
    return value;
}
