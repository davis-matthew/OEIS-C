/*
 * A045804  4-ish numbers (end in 09, 41, 63, 87).
 * https://oeis.org/A045804
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 9 + 100 * floor((n-1)/4) + [0,32,54,78][(n-1)%4]
 * Range:     u8 n <= 10, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Number of complete 4‑term cycles preceding n */
    idx_t cycle = (n - 1) / 4;
    /* Position inside the current cycle (0‑based) */
    idx_t rem   = (n - 1) % 4;

    /* Base value */
    seq_t result = 9;

    /* Add the offset within the cycle */
    if (rem == 1) {
        result += (seq_t)32;
    } else if (rem == 2) {
        result += (seq_t)54;
    } else if (rem == 3) {
        result += (seq_t)78;
    }

    /* Add 100 for each full cycle */
    seq_t cycle_offset = (seq_t)100 * (seq_t)cycle;
    result += cycle_offset;

    return result;
}
