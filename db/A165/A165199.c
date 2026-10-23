/*
 * A165199  a(n) is obtained by flipping every second bit in the binary
 *          representation of n starting at the second-most significant bit
 *          and on downwards.
 * https://oeis.org/A165199
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn, look
 * Formula:   a(n) = n XOR mask, where mask has 1 bits at positions * len-2,
 *            len-4, len-6, ... (i.e. every second bit starting from the *
 *            second‑most significant bit). a(0) = 0. This is the definition
 *            of * OEIS A165199
 * Range:     u8 n <= 255, u16 n <= 1023, u32 n <= 1023, u64 n <= 1023,
 *            u128 n <= 1023, big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_log2_uint64.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* base case */

    /* length of the binary representation of n */
    const idx_t len = floor_log2_uint64(n) + 1;

    seq_t mask = 0;

    /* Build mask: flip every second bit starting at position len-2. */
    if (len >= 2) {
        idx_t pos = len - 2;
        while (1) {
            mask |= ((seq_t)1 << pos);
            if (pos < 2) break;   /* stop when next position would be negative */
            pos -= 2;
        }
    }

    /* Apply the mask to n. */
    return ((seq_t)n) ^ mask;
}
