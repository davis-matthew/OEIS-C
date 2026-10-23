/*
 * A044590  Numbers n such that string 2,6 occurs in the base 8 representation
 *          of n but not of n+1.
 * https://oeis.org/A044590
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th positive integer m such that the base‑8
 *            representation of m contains the substring “26” while the
 *            representation of m+1 does not
 * Range:     u8 n <= 5, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   has26_8
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/has26_8.h"

/* OEIS A044590 */
seq_t a(idx_t n)
{
    idx_t cnt = 0;
    uint64_t val = 0;

    while (cnt < n) {
        if (has26_8(val) && !has26_8(val + 1)) {
            ++cnt;
            if (cnt == n)
                return (seq_t)val;
        }
        ++val;
    }
    /* Should never reach here for valid n ≥ 1 */
    return (seq_t)0;
}
