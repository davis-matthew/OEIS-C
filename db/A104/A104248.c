/*
 * A104248  Lengths of successive runs of 1's in the Thue-Morse sequence
 *          A010060.
 * https://oeis.org/A104248
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A026465(2*n) – lengths of successive runs of 1's in the
 *            Thue‑Morse sequence
 * Range:     u8 n <= 9929, u16 n <= 9929, u32 n <= 9929, u64 n <= 9929,
 *            u128 n <= 9929, big any n
 * Helpers:   thue_bit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/thue_bit.h"

/* External function defining the sequence */
seq_t a(idx_t n)
{
    idx_t i       = 0;
    unsigned int prev_bit = thue_bit(i);
    idx_t run_len = 1;          /* length of current run */
    idx_t count   = 0;          /* number of 1‑runs seen so far */

    for (;; ++i) {
        unsigned int bit = thue_bit(i);
        if (bit == prev_bit) {
            ++run_len;
        } else {
            if (prev_bit == 1) {   /* a run of 1's just finished */
                ++count;
                if (count == n)
                    return (seq_t)run_len;
            }
            prev_bit = bit;
            run_len = 1;
        }
    }
}
