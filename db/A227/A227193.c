/*
 * A227193  Difference of (product of runlengths of 1-bits) and (product of
 *          runlengths of 0-bits) in binary representation of n.
 * https://oeis.org/A227193
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (product of lengths of runs of 1‑bits) - (product of
 *            lengths of runs of 0‑bits) in the binary representation of n
 * Range:     i8 n <= 9515, i16 n <= 9515, i32 n <= 9515, i64 n <= 9515,
 *            i128 n <= 9515, big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/floor_log2_uint64.h"

seq_t a(idx_t n)
{
    /* Handle the trivial case n == 0 */
    if (n == 0) {
        return 0;
    }

    /* Find the index of the most significant 1-bit. */
    idx_t msb = floor_log2_uint64(n);

    /* Products of run lengths.  Empty products are 1. */
    seq_t prod1 = 1;
    seq_t prod0 = 1;

    /* Current run information. */
    int prev_bit = (int)((n >> msb) & 1);
    idx_t run_len = 1;

    /* Scan bits from the next lower bit down to bit 0. */
    for (idx_t i = msb; i > 0; i--) {
        int bit = (int)((n >> (i - 1)) & 1);
        if (bit == prev_bit) {
            /* Continue the current run. */
            run_len++;
        } else {
            /* Finish the previous run and start a new one. */
            if (prev_bit == 1) {
                prod1 *= (seq_t)run_len;
            } else {
                prod0 *= (seq_t)run_len;
            }
            prev_bit = bit;
            run_len = 1;
        }
    }

    /* Final run after the loop. */
    if (prev_bit == 1) {
        prod1 *= (seq_t)run_len;
    } else {
        prod0 *= (seq_t)run_len;
    }

    /* Return the difference. */
    return prod1 - prod0;
}
