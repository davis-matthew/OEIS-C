/*
 * A087116  Number of maximal groups of consecutive zeros in binary
 *          representation of n.
 * https://oeis.org/A087116
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of maximal groups of consecutive zeros in the
 *            binary * representation of n (the representation has no leading
 *            zeros; a(0) = 1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Special case: 0 is represented by the single character "0",
       which contains exactly one zero‑run. */
    if (n == 0)
        return (seq_t)1;

    /* Find the index of the most significant 1 bit. */
    idx_t msb = 0;
    idx_t tmp = n;
    while ((tmp >> 1) != 0) {
        tmp >>= 1;
        msb++;
    }

    seq_t count = (seq_t)0;
    bool in_zero_run = false;   /* are we inside a run of zeros? */
    bool prev_bit_one = false;  /* was the previous bit a 1? */

    /* Scan bits from most significant to least significant. */
    for (idx_t i = msb;; i--) {
        bool bit = ((n >> i) & 1U) != 0;
        if (bit) {
            in_zero_run = false;   /* any zero run ends before this 1 */
            prev_bit_one = true;   /* next zero will start a new run */
        } else {
            if (prev_bit_one && !in_zero_run) {
                count++;            /* start of a new zero run */
                in_zero_run = true;
            }
            prev_bit_one = false;
        }
        if (i == 0)
            break;
    }

    return count;
}
