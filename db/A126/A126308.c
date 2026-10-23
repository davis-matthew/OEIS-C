/*
 * A126308  Delete '10'-substrings in the binary expansion of n.
 * https://oeis.org/A126308
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* Description: delete all non‑overlapping '10' substrings from the binary representation of n. */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Compute bit length of n. */
    idx_t tmp = n;
    idx_t bitlen = 0;
    while (tmp) {
        bitlen++;
        tmp >>= 1;
    }

    seq_t out = 0;
    size_t out_bits = 0;
    int prev_bit = -1;   /* -1 indicates no previous bit yet */

    /* Scan bits from most significant to least significant. */
    for (idx_t i = bitlen; i-- > 0;) {
        int cur = (int)((n >> i) & 1);   /* current bit (0 or 1) */

        if (prev_bit == 1 && cur == 0) {
            /* Found the pattern '10': remove the preceding 1
               from the result and skip this 0. */
            if (out_bits > 0) {
                out >>= 1;
                out_bits--;
            }
        } else {
            /* Append current bit to the output. */
            out = (out << 1) | (seq_t)cur;
            out_bits++;
        }

        prev_bit = cur;
    }

    return out;
}
