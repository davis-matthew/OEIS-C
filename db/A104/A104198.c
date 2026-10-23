/*
 * A104198  Smallest number greater than n, having in its decimal
 *          representation more distinct digits than n, that are also
 *          different from n's digits.
 * https://oeis.org/A104198
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, fini
 * Formula:   a(n) = smallest m > n such that digit_mask(m) & digit_mask(n) ==
 *            0 (disjoint digit sets) and popcount(digit_mask(m)) >
 *            popcount(digit_mask(n)), with the special rule that
 *            digit_mask(0) includes digit 0
 * Range:     u8 n <= 11, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   digit_mask
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/digit_mask.h"

seq_t a(idx_t n)
{
    /* digit set of n (0 treated specially) */
    uint16_t mask_n = digit_mask((seq_t)n);
    if (n == 0u)           /* ensure 0 contributes digit 0 */
        mask_n |= 1u;      /* 1u == 1 << 0 */

    unsigned pc_n = __builtin_popcount((unsigned)mask_n);

    /* If n already uses 5 or more distinct digits, no disjoint set can
       provide more distinct digits.  Return 0 to signal "no term". */
    if (pc_n >= 5)
        return 0;

    idx_t m = n + 1;
    for (;;) {
        uint16_t mask_m = digit_mask((seq_t)m);
        /* digits of m must be disjoint from digits of n */
        if ((mask_m & mask_n) == 0u) {
            unsigned pc_m = __builtin_popcount((unsigned)mask_m);
            if (pc_m > pc_n)          /* more distinct digits than n */
                return (seq_t)m;
        }
        ++m;
    }
}
