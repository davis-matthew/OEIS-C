/*
 * A021779  Decimal expansion of 1/775.
 * https://oeis.org/A021779
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = ⌊10^(n+1) / 775⌋ mod 10 -- nth decimal digit of 1/775
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Compute decimal digits of 1/775 by successive integer division */
    uint64_t rem = 1;  /* remainder before division, initially 1/775 */
    for (idx_t i = 0; i <= n; ++i) {
        uint64_t prod = rem * 10ULL;      /* multiply by 10 each step   */
        seq_t digit = prod / 775ULL;      /* next decimal digit         */
        rem = prod % 775ULL;              /* update remainder           */
        if (i == n) return digit;         /* return the requested digit */
    }
    return 0;  /* unreachable, keeps compiler happy */
}
