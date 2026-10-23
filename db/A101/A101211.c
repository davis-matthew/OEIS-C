/*
 * A101211  Triangle read by rows: n-th row is length of run of leftmost 1's,
 *          followed by length of run of 0's, followed by length of run of
 *          1's, etc., in the binary representation of n.
 * https://oeis.org/A101211
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, tabf
 * Formula:   a(n) = the n‑th entry of the triangle read by rows of
 *            run‑lengths of the binary representation of positive integers,
 *            starting at 1
 * Range:     u8 n <= 5120, u16 n <= 5120, u32 n <= 5120, u64 n <= 5120,
 *            u128 n <= 5120, big any n
 * Helpers:   bitlen
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/bitlen.h"

seq_t a(idx_t n)
{
    idx_t count = 0;                /* how many run lengths have been produced so far */

    for (idx_t x = 1; ; ++x) {      /* iterate over the rows (numbers) */
        int msb = (int)bitlen(x) - 1;   /* position of the most significant bit */
        int curr_bit = 1;              /* highest bit of any positive integer is 1 */
        idx_t run_len = 1;             /* the current run starts with that bit */

        for (int i = msb - 1; i >= 0; --i) {
            int bit = (x >> i) & 1;    /* next bit in the binary string */
            if (bit == curr_bit) {
                ++run_len;             /* extend the current run */
            } else {
                ++count;                /* finished a run, emit it */
                if (count == n) {
                    return (seq_t)run_len;
                }
                curr_bit = bit;        /* start a new run */
                run_len = 1;
            }
        }

        /* emit the last run of this row */
        ++count;
        if (count == n) {
            return (seq_t)run_len;
        }
    }
}
