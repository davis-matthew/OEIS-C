/*
 * A175267  a(n) = the minimum number of 0's that, if removed from the binary
 *          representation of n, leaves a palindrome.
 * https://oeis.org/A175267
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = minimum number of 0's that must be removed from the
 *            binary representation of n to leave a palindrome
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_log2_uint64.h"

seq_t a(idx_t n)
{
    /* n = 0 is already a palindrome of a single zero */
    if (n == 0)
        return 0;

    /* Length of the binary representation (most significant bit
       inclusive). */
    const idx_t msb = floor_log2_uint64(n);   /* highest set bit index (0‑based) */
    const idx_t L = msb + 1;                  /* number of bits */

    seq_t removed = 0;
    idx_t l = 0;               /* left pointer (from MSB)   */
    idx_t r = L - 1;           /* right pointer (from LSB)  */

    while (l < r) {
        /* extract bits at positions l and r */
        const bool bit_l = ((uint64_t)n >> (L - 1 - l)) & 1ULL;
        const bool bit_r = ((uint64_t)n >> (L - 1 - r)) & 1ULL;

        if (bit_l == bit_r) {            /* bits match: keep them */
            ++l;
            --r;
        } else {                         /* mismatch: delete the zero */
            ++removed;
            if (bit_r == 0) {
                /* zero at the right end */
                --r;
            } else {
                /* zero at the left end */
                ++l;
            }
        }
    }

    return removed;
}
